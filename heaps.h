#ifndef HEAPS
#define HEAPS

#include "vm_riskxvii.h"
#include <math.h>

#define HEAP_SIZE (128)

struct node {
    uint32_t address;
    uint32_t memory; // In bits
    uint32_t num_banks;
    int8_t array[64];
};

extern void initialise_heaps(int size, struct node* list);
extern int get_address(uint32_t index);
extern int get_index(uint32_t address, struct node* list);
extern void my_malloc(uint32_t malloc_size, struct node* list, struct binary* input);
extern void my_free(uint32_t address, struct node* list, struct binary* input);
extern int address_allocated(uint32_t address, struct node* list);

#endif