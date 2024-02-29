#ifndef VM_RISKXVII
#define VM_RISKXVII

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INST_MEM_SIZE (256)
#define DATA_MEM_SIZE (1024)
#define REGISTER_SIZE (32)
#define BIT_SIZE (32)

struct binary {
    int inst_mem[INST_MEM_SIZE];
    char data_mem[DATA_MEM_SIZE];
    int reg[REGISTER_SIZE];
    int PC;
    int prev_address; // Storing previous address of malloc in memory
    int prev_malloc; // Storing previous malloc size in memory
};

#endif
