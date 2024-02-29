#ifndef HELPER
#define HELPER

#include "vm_riskxvii.h"
#include "operations.h"

extern void register_dump(struct binary* input);
extern int get_bits(int bin, int p, int n);
extern void get_operation(char* opcode, char* operation, struct binary* input);
void illegal_operation(struct binary* input);

#endif