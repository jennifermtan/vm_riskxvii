#include "vm_riskxvii.h"
#include "operations.h"
#include "helper.h"
#include "heaps.h"

// Jump and Link
void jal(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 21, 10) >> 20) | (get_bits(binary, 20, 1) >> 9) | (get_bits(binary, 12, 8)) | (get_bits(binary, 31, 1) >> 11);
    if (rd != 0) {
        input->reg[rd] = input->PC + 4;
    }
    input->PC = input->PC + imm;
}

// Branch if Greater Than or Equal UNSIGNED
void bgeu(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);

    if ((uint32_t) input->reg[rs1] >= (uint32_t) input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Branch if Greater Than or Equal
void bge(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);

    if (input->reg[rs1] >= input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Branch if Less Than UNSIGNED
void bltu(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);

    if ((uint32_t) input->reg[rs1] < (uint32_t) input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Branch if Less Than
void blt(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);

    if (input->reg[rs1] < input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Branch if Not Equal
void bne(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);
    if (input->reg[rs1] != input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Branch if Equal
void beq(int binary, struct binary* input) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    // Bit 0 = always 0 because (imm << 1)
    int imm = (get_bits(binary, 8, 4) >> 7) | (get_bits(binary, 25, 6) >> 20) | (get_bits(binary, 7, 1) << 4) | (get_bits(binary, 31, 1) >> 19);
    if (input->reg[rs1] == input->reg[rs2]) {
        input->PC = input->PC + imm;
    } else {
        input->PC += 4;
    }
}

// Store Word
void sw(int binary, struct binary* input, struct node* list) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    int imm = (get_bits(binary, 7, 5) >> 7) | (get_bits(binary, 25, 7) >> 20); 
    int address = input->reg[rs1] + imm;
    int index1 = address - DATA_MEM_SIZE;
    int index2 = index1 + 1;
    int index3 = index2 + 1;
    int index4 = index3 + 1;
    int val1 = get_bits(input->reg[rs2], 0, 8);
    int val2 = get_bits(input->reg[rs2], 8, 8) >> 8;
    int val3 = get_bits(input->reg[rs2], 16, 8) >> 16;
    int val4 = get_bits(input->reg[rs2], 24, 8) >> 24;
    
    if (address == 0x0800) {
        printf("%c", input->reg[rs2]);
    } else if (address == 0x0804) {
        printf("%d", input->reg[rs2]);
    } else if (address == 0x0808) {
        printf("%x", (uint32_t) input->reg[rs2]);
    } else if (address == 0x080C) {
        printf("CPU Halt Requested\n");
        exit(0);
    } else if (address == 0x0820) {
        printf("%x", input->PC);
    } else if (address == 0x0824) {
        register_dump(input);
    } else if (address == 0x0828) {
        printf("%x", (uint32_t) input->data_mem[input->reg[rs2]]);
    } else if (address == 0x0830) {
        my_malloc(input->reg[rs2], list, input);
    } else if (address == 0x0834) {
        my_free(input->reg[rs2], list, input);
    } else if ((address >= 0xb700) && (address <= 0xd700)) {
        if (address_allocated(address, list)) {
            list[get_index(address, list)].array[0] = val1;
            list[get_index(address, list)].array[1] = val2;
            list[get_index(address, list)].array[2] = val3;
            list[get_index(address, list)].array[3] = val4;
        } else {
            illegal_operation(input);
        }
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else {
        input->data_mem[index1] = val1;
        input->data_mem[index2] = val2;
        input->data_mem[index3] = val3;
        input->data_mem[index4] = val4;
    }
    input->PC += 4;
}

// Store Half Word
void sh(int binary, struct binary* input, struct node* list) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    int imm = (get_bits(binary, 7, 5) >> 7) | (get_bits(binary, 25, 7) >> 20); 
    int address = input->reg[rs1] + imm;
    int index1 = address - DATA_MEM_SIZE;
    int index2 = index1 + 1;
    int val1 = get_bits(input->reg[rs2], 0, 8);
    int val2 = get_bits(input->reg[rs2], 8, 8) >> 8;

    if (address == 0x0800) {
        printf("%c", input->reg[rs2]);
    } else if (address == 0x0804) {
        printf("%d", input->reg[rs2]);
    } else if (address == 0x0808) {
        printf("%x", (uint32_t) input->reg[rs2]);
    } else if (address == 0x080C) {
        printf("CPU Halt Requested\n");
        exit(0);
    } else if (address == 0x0820) {
        printf("%x", input->PC);
    } else if (address == 0x0824) {
        register_dump(input);
    } else if (address == 0x0828) {
        printf("%x", (uint32_t) input->data_mem[input->reg[rs2]]);
    } else if (address == 0x0830) {
        my_malloc(input->reg[rs2], list, input);
    } else if (address == 0x0834) {
        my_free(input->reg[rs2], list, input);
    } else if ((address >= 0xb700) && (address <= 0xd700)) {
        if (address_allocated(address, list)) {
            list[get_index(address, list)].array[0] = val1;
            list[get_index(address, list)].array[1] = val2;
        } else {
            illegal_operation(input);
        }
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else {
        input->data_mem[index1] = val1;
        input->data_mem[index2] = val2;
    }
    input->PC += 4;
}

// Store Byte
void sb(int binary, struct binary* input, struct node* list) {
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    int imm = (get_bits(binary, 7, 5) >> 7) | (get_bits(binary, 25, 7) >> 20); 
    int address = input->reg[rs1] + imm;

    if (address == 0x0800) {
        printf("%c", input->reg[rs2]);
    } else if (address == 0x0804) {
        printf("%d", input->reg[rs2]);
    } else if (address == 0x0808) {
        printf("%x", (uint32_t) input->reg[rs2]);
    } else if (address == 0x080C) {
        printf("CPU Halt Requested\n");
        exit(0);
    } else if (address == 0x0820) {
        printf("%x", input->PC);
    } else if (address == 0x0824) {
        register_dump(input);
    } else if (address == 0x0828) {
        printf("%x", (uint32_t) input->data_mem[input->reg[rs2]]);
    } else if (address == 0x0830) {
        my_malloc(input->reg[rs2], list, input);
    } else if (address == 0x0834) {
        my_free(input->reg[rs2], list, input);
    } else if ((address >= 0xb700) && (address <= 0xd700)) {
        if (address_allocated(address, list)) {
            list[get_index(address, list)].array[0] = input->reg[rs2];
        } else {
            illegal_operation(input);
        }
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else {
        input->data_mem[address - DATA_MEM_SIZE] = input->reg[rs2];
    }
    input->PC += 4;
}

// Load Half Word UNSIGNED
void lhu(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    int address = input->reg[rs1] + imm;
    int index1 = address - DATA_MEM_SIZE;
    int index2 = index1 + 1; //Index for the next 8 bits
    uint32_t value = (uint32_t) (input->data_mem[index1] | (input->data_mem[index2] << 8));

    if (address == 0x0812) {
        scanf("%u", &input->reg[rd]);
    } else if (address == 0x0816) {
        scanf("%u", &input->reg[rd]);
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else if (rd != 0) {
        input->reg[rd] = value;
    } 
    input->PC += 4;
    
}

// Load Byte UNSIGNED
void lbu(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = (get_bits(binary, 15, 5) >> 15);
    int imm = (get_bits(binary, 20, 12) >> 20);
    int address = input->reg[rs1] + imm;

    if (address == 0x0812) {
        scanf("%u", &input->reg[rd]);
    } else if (address == 0x0816) {
        scanf("%u", &input->reg[rd]);
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else if (rd != 0) {
        input->reg[rd] = (uint32_t) input->data_mem[address - DATA_MEM_SIZE];
    }
    input->PC += 4;
}

// Load Word
void lw(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    int address = input->reg[rs1] + imm;
    int index1 = address - DATA_MEM_SIZE;
    int index2 = index1 + 1; //Index for the next 8 bits
    int index3 = index2 + 1;
    int index4 = index3 + 1;
    int value = (input->data_mem[index1] & 0xff) | ((input->data_mem[index2] & 0xff) << 8) | ((input->data_mem[index3] & 0xff) << 16) | ((input->data_mem[index4] & 0xff) << 24);

    if (address == 0x0812) {
        scanf("%d", &input->reg[rd]);
    } else if (address == 0x0816) {
        scanf("%d", &input->reg[rd]);
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else if (rd != 0) {
        input->reg[rd] = value;
    }
    input->PC += 4;
}

// Load Half Word
void lh(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    int address = input->reg[rs1] + imm;
    int index1 = address - DATA_MEM_SIZE;
    int index2 = index1 + 1; //Index for the next 8 bits
    int value = (input->data_mem[index1] & 0xff) | (input->data_mem[index2] << 8);

    if (address == 0x0812) {
        scanf("%d", &input->reg[rd]);
    } else if (address == 0x0816) {
        scanf("%d", &input->reg[rd]);
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else if (rd != 0) {
        input->reg[rd] = value;
    }
    input->PC += 4;
}

// Load Byte
void lb(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    int address = input->reg[rs1] + imm;

    if (address == 0x0812) {
        scanf("%d", &input->reg[rd]);
    } else if (address == 0x0816) {
        scanf("%d", &input->reg[rd]);
    } else if (address >= 0x0850) {
        illegal_operation(input);
    } else if (rd != 0) {
        input->reg[rd] = input->data_mem[address - DATA_MEM_SIZE];
    }
    input->PC += 4;
}

// Load Upper Immediate
void lui(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int imm = get_bits(binary, 12, 20);

    if (rd != 0) {
        input->reg[rd] = imm;
    }
    input->PC += 4;
}

// Jump and Link Register
void jalr(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = input->PC + 4;
    }
    input->PC = input->reg[rs1] + imm;
}

// Set if Smaller Immediate UNSIGNED
void sltiu(int binary, struct binary* input) {
    int rd = (get_bits(binary, 7, 5) & 0xff) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = ((uint32_t) input->reg[rs1] < (uint32_t) imm) ? 1 : 0;
    }
    input->PC += 4;
}

// Set if Smaller Immediate
void slti(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = (input->reg[rs1] < imm) ? 1 : 0;
    }
    input->PC += 4;
}

// AND immediate
void andi(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] & imm;
    }
    input->PC += 4;
}

// OR immediate
void ori(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] | imm;
    }
    input->PC += 4;
}

// XOR immediate
void xori(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] ^ imm;
    }
    input->PC += 4;
}

// Add immediate
void addi(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int imm = get_bits(binary, 20, 12) >> 20;
    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] + imm;
    }
    input->PC += 4;
}

// Set if Smaller UNSIGNED
void sltu(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = ((uint32_t) input->reg[rs1] < (uint32_t) input->reg[rs2]) ? 1 : 0;
    }
    input->PC += 4;
}

// Set if Smaller
void slt(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;
    
    if (rd != 0) {
        input->reg[rd] = (input->reg[rs1] < input->reg[rs2]) ? 1 : 0;
    }
    input->PC += 4;
}

// Rotate right
void sra(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        // After shifting, put last bits of rs1 in front
        input->reg[rd] = (input->reg[rs1] >> input->reg[rs2]) | (input->reg[rs1] << (BIT_SIZE - input->reg[rs2]));
    }
    input->PC += 4;
}

// Shift right
void srl(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = (uint32_t) (input->reg[rs1]) >> input->reg[rs2];
    }
    input->PC += 4;
}

// Shift left
void sll(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] << input->reg[rs2];
    }
    input->PC += 4;
}

// AND
void and(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] & input->reg[rs2];
    }
    input->PC += 4;
}

// OR
void or(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] | input->reg[rs2];
    }
    input->PC += 4;
}

// XOR 
void xor(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] ^ input->reg[rs2];
    }
    input->PC += 4;
}

// Subtract
void sub(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] - input->reg[rs2];
    }
    input->PC += 4;
}

// Add
void add(int binary, struct binary* input) {
    int rd = get_bits(binary, 7, 5) >> 7;
    int rs1 = get_bits(binary, 15, 5) >> 15;
    int rs2 = get_bits(binary, 20, 5) >> 20;

    if (rd != 0) {
        input->reg[rd] = input->reg[rs1] + input->reg[rs2];
    }
    input->PC += 4;
}