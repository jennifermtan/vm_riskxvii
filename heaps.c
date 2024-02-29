#include "heaps.h"
#include "helper.h"

// Initialise a heap bank of nodes of a cer†ain size
void initialise_heaps(int size, struct node* list) {
    // Initialising the node
    struct node n = {
        .address = 0,
        .memory = 0,
        .num_banks = 0,
        .array = {0}
    };

    for (int i = 0; i < size; i++) {
        list[i] = n;
        list[i].address = get_address(i);
        list[i].memory = 0;
    }
}

// Returns the starting address of a node
int get_address(uint32_t index) {
    return ((index * 64) + 0xb700);
}

// Returns the index of a node
int get_index(uint32_t address, struct node* list) {
    return (address - 0xb700) / 64;
}

// malloc() command to allocate memory according to the size requested
void my_malloc(uint32_t malloc_size, struct node* list, struct binary* input) {
    if (malloc_size > 8192) {
        input->reg[28] = 0;
        return;
    }

    if (malloc_size <= 64) {
        int allocated = 0;
        // Loop through the list and check if each node has stored memory or not
        for (int i = 0; i < HEAP_SIZE; i++) {
            if (list[i].memory == 0 && !allocated) {
                list[i].memory = malloc_size;
                input->reg[28] = list[i].address;
                list[i].num_banks = 1;
                allocated = 1;
            }
        }
        if (!allocated) {
            input->reg[28] = 0;
        }
    } else {
        int num_banks = ceil(malloc_size/64);
        int counter = 0;
        int start_index = 0;
        // Check for consecutive heap banks
        for (int i = 0; i < HEAP_SIZE; i++) {
            if (list[i].memory == 0) {
                if (counter == num_banks) {
                    start_index = i - num_banks;
                    break;
                }
                counter++;
            } else {
                counter = 0;
            }
        }
    
        if (counter < num_banks) {
            input->reg[28] = 0;
        } else {
            for (int i = 0; i < num_banks; i++) {
                if (i == (num_banks - 1)) {
                    list[start_index + i].memory = malloc_size - (i * 64); // If it is the last index, allocate the rest of the memory
                    list[start_index].num_banks = num_banks - i;
                } else {
                    list[start_index + i].memory = 64;
                    list[start_index].num_banks = num_banks - i;
                }
            }
            list[start_index].num_banks = num_banks;
            input->reg[28] = list[start_index].address;
        }
    }
}

// free() command to free the memory at a specific address
void my_free(uint32_t address, struct node* list, struct binary* input) {
    int index = get_index(address, list);
    if (list[index].num_banks == 1) {
        if (list[index].memory != 0) { // If there is memory allocated
            list[index].memory = 0;
            list[index].num_banks = 0;
        } else {
            illegal_operation(input);
        }
    } else {
        for (int i = 0; i < list[index].num_banks; i++) {
            if (list[index + i].memory != 0) { // If there is memory allocated
                list[index + i].memory = 0;
                list[index].num_banks = 0;
            } else {
                illegal_operation(input);
            }
        }
    }
}

// Checks if the address allocated in the heap bank
int address_allocated(uint32_t address, struct node* list) {
    int index = get_index(address, list);
    int end_address = list[index].address + list[index].memory - 1;

    if ((address >= list[index].address) && (address <= end_address) && (list[index].address != 0)) {
        return 1;
    }
    return 0;
}