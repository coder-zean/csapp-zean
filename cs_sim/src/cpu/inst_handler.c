#include "cpu/inst_handler.h"



void init_handler_table() {
    handler_table[0] = mov_handler;
    handler_table[1] = add_handler;
    handler_table[2] = sub_handler;
    handler_table[3] = add_handler;
    handler_table[4] = push_handler;
    handler_table[5] = pop_handler;
}