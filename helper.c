#include "helper.h"
#include "vm_riskxvii.h"

// Get n bits from position p
// https://stackoverflow.com/questions/21540158/how-do-i-extract-bits-from-32-bit-number
int get_bits(int bin, int p, int n)
{   
    return (((1 << n) - 1) << p) & bin; // Set n bits to 1 from p to p + n
}

// Stores the respective operation in char* operation based on the encoding
void get_operation(char* opcode, char* operation, struct binary* input) {
    if ((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0110011) == 0b0) {
        strcpy(opcode, "R");
    } else if ((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0100011) == 0b0) {
        strcpy(opcode, "S");
    } else if ((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b1100011) == 0b0) {
        strcpy(opcode, "SB");
    } else if ((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0110111) == 0b0) {
        strcpy(opcode, "U");
        strcpy(operation, "lui");
    } else if ((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b1101111) == 0b0) {
        strcpy(opcode, "UJ");
        strcpy(operation, "jal");
    }
    // Compare func7 and func3 for type R
    if (strcmp(opcode, "R") == 0) {
        if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) ^ 0b0000000) == 0b0)) {
            strcpy(operation, "add");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0100000) == 0b0)) {
            strcpy(operation, "sub");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b100) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "xor");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b110) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "or");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b111) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "and");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b001) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "sll");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b101) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "srl");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b101) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0100000) == 0b0)) {
            strcpy(operation, "sra");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b010) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "slt");
        } else if (((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 25, 7) >> 25 ^ 0b0000000) == 0b0)) {
            strcpy(operation, "sltu");
        }
    }
    // Compare func3 for type S
    else if (strcmp(opcode, "S") == 0) {
        if ((get_bits(input->inst_mem[input->PC/4], 12, 3) ^ 0b000) == 0b0) {
            strcpy(operation, "sb");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b001) == 0b0) {
            strcpy(operation, "sh");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b010) == 0b0) {
            strcpy(operation, "sw");
        } 
    }
    // Compare func3 for type SB
    else if (strcmp(opcode, "SB") == 0) {
        if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0) {
            strcpy(operation, "beq");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b001) == 0b0) {
            strcpy(operation, "bne");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b100) == 0b0) {
            strcpy(operation, "blt");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b110) == 0b0) {
            strcpy(operation, "bltu");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b101) == 0b0) {
            strcpy(operation, "bge");
        } else if ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b111) == 0b0) {
            strcpy(operation, "bgeu");
        } 
    }
    // Compare opcode and func3 for type I
    else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0)) {
        strcpy(operation, "addi");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b100) == 0b0)) {
        strcpy(operation, "xori");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b110) == 0b0)) {
        strcpy(operation, "ori");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b111) == 0b0)) {
        strcpy(operation, "andi");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b010) == 0b0)) {
        strcpy(operation, "slti");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0010011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b011) == 0b0)) {
        strcpy(operation, "sltiu");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0000011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0)) {
        strcpy(operation, "lb");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0000011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b001) == 0b0)) {
        strcpy(operation, "lh");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0000011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b010) == 0b0)) {
        strcpy(operation, "lw");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0000011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b100) == 0b0)) {
        strcpy(operation, "lbu");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b0000011) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b101) == 0b0)) {
        strcpy(operation, "lhu");
    } else if (((get_bits(input->inst_mem[input->PC/4], 0, 7) ^ 0b1100111) == 0b0) && ((get_bits(input->inst_mem[input->PC/4], 12, 3) >> 12 ^ 0b000) == 0b0)) {
        strcpy(operation, "jalr");
    }
}

// Prints PC and all values of registers in hexadecimal
void register_dump(struct binary* input) {
    printf("PC = 0x%08x;\n", input->PC);
    for (int i = 0; i < REGISTER_SIZE; i++) {
        printf("R[%d] = ", i);
        printf("0x%08x;\n", input->reg[i]);
    }
}

// Executes the illegal operation error handling, prints out register dump
void illegal_operation(struct binary* input) {
    int value = input->inst_mem[(input->PC)/4];
    printf("Illegal Operation: 0x%08x\n", value);
    register_dump(input);
    exit(0);
}