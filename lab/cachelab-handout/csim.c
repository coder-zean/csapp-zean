#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX_SIZE 100
#define MASK 0xffffffff

extern char *optarg;

typedef struct cache_line_t {
    long long tag;
    struct cache_line_t* next;
    struct cache_line_t* prev;
} CacheLine;

typedef struct cache_set_t {
    int cur_cache_line_num;
    CacheLine* dummy_head;
    CacheLine* dummy_tail;
} CacheSet;

typedef struct cache_t {
    CacheSet* cache_set;
} Cache;

CacheLine* NewCacheLine(long long tag, CacheLine* prev, CacheLine* next) {
    CacheLine* cache_line = malloc(sizeof(CacheLine));
    cache_line->next = next;
    cache_line->prev = prev;
    cache_line->tag = tag;
    return cache_line;
}

Cache* NewCache(int set_bits) {
    Cache* cache = malloc(sizeof(Cache));
    cache->cache_set = malloc(sizeof(CacheSet) * (1 << set_bits));
    for (int i = 0; i < (1 << set_bits); i++) {
        cache->cache_set[i].cur_cache_line_num = 0;
        cache->cache_set[i].dummy_head = NewCacheLine(0, NULL, NULL);
        cache->cache_set[i].dummy_tail = NewCacheLine(0, NULL, NULL);
        cache->cache_set[i].dummy_head->next = cache->cache_set[i].dummy_tail;
        cache->cache_set[i].dummy_tail->prev = cache->cache_set[i].dummy_head;
    }
    return cache;
}

void MoveCacheLine(CacheLine* src, CacheLine* dst) {
    if (src->next) {
        src->next->prev = src->prev;
        src->prev->next = src->next;
    }
    dst->next->prev = src;
    src->next = dst->next;
    dst->next = src;
    src->prev = dst;
}

void PrintHelperMsg() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

// 分别表示cache组所占位数，每组有多少个cache line，组内偏移量
int set_num = 1, cache_line_num = 1, block_bytes = 8;
int is_verbose = 0;
FILE* trace_file = NULL;

void ParseOpt(int argc, char* argv[]) {
    int ch = 0;
    while ((ch = getopt(argc, argv, "vhs:E:b:t:")) != -1) {
        switch (ch) {
            case 'h':
                PrintHelperMsg();
                exit(0);
            case 'v':
                is_verbose = 1;
                break;
            case 's':
                set_num = atoi(optarg);
                break;
            case 'E':
                cache_line_num = atoi(optarg);
                break;
            case 'b':
                block_bytes = atoi(optarg);
                break;
            case 't':
                trace_file = fopen(optarg, "r");
                break;
            default:
                PrintHelperMsg();
                exit(0);
        }
    }
}

void DealOp(char op, char* print_str, int* hit) {
    if (op == 'M') {
        (*hit)++;
        if (is_verbose) {
            printf(" %s hit\n", print_str);
        }   
    } else {
        if (is_verbose) {
            printf(" %s\n", print_str);
        }
    }
}

int main(int argc, char* argv[]) {
    ParseOpt(argc, argv);
    if (trace_file == NULL) {
        printf("Trace file can not open! Please check the file name.\n");
        return 0;
    }
    int hit = 0, misses = 0, evictions = 0;
    Cache* cache = NewCache(set_num);
    
    char line_str[LINE_MAX_SIZE];
    while (fgets(line_str, LINE_MAX_SIZE, trace_file) != NULL) {
        if (line_str[1] != 'L' && line_str[1] != 'S' && line_str[1] != 'M') {
            continue;
        }
        int len = strlen(line_str);
        line_str[len - 1] = '\0';
        char op = line_str[1];
        if (is_verbose) {
            printf("%s", line_str);
        }

        int i = 3;
        while (line_str[i] != ',') {
            line_str[i - 3] = line_str[i];
            i++;
        }
        line_str[i - 3] = '\0';

        long long addr = strtoll(line_str, NULL, 16);
        long long group = (addr >> block_bytes) & (MASK >> (64 - set_num));
        long long tag = addr >> (block_bytes + set_num);

        CacheSet* cur_set = &(cache->cache_set[group]);
        if (cur_set->cur_cache_line_num != 0) {
            CacheLine* work = cur_set->dummy_head->next;
            int is_hit = 0;
            while (work != cur_set->dummy_tail) {
                if (work->tag == tag) {
                    hit++;
                    MoveCacheLine(work, cur_set->dummy_head);
                    is_hit = 1;
                    DealOp(op, "hit", &hit);
                    break;
                }
                work = work->next;
            }
            if (!is_hit) {
                misses++;
                CacheLine* new_line = NewCacheLine(tag, NULL, NULL);
                if (cur_set->cur_cache_line_num == cache_line_num) {
                    DealOp(op, "miss eviction", &hit);
                    evictions++;
                    CacheLine* free_node = cur_set->dummy_tail->prev;
                    free_node->prev->next = cur_set->dummy_tail;
                    cur_set->dummy_tail->prev = free_node->prev;
                    free(free_node);
                    MoveCacheLine(new_line, cur_set->dummy_head);
                } else {
                    DealOp(op, "miss", &hit);
                    MoveCacheLine(new_line, cur_set->dummy_head);
                    cur_set->cur_cache_line_num++;
                }
            } 
        } else {
            DealOp(op, "miss", &hit);
            misses++;
            CacheLine* new_line = NewCacheLine(tag, cur_set->dummy_head, cur_set->dummy_tail);
            cur_set->dummy_head->next = new_line;
            cur_set->dummy_tail->prev = new_line;
            cur_set->cur_cache_line_num++;
        }
    }

    printSummary(hit, misses, evictions);
    return 0;
}
