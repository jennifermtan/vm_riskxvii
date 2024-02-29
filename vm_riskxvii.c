#include "operations.h"
#include "vm_riskxvii.h"
#include "helper.h"
#include "heaps.h"

int main(int argc, char *argv[]) {

    FILE * file = fopen(argv[1], "rb");
    if (file == NULL) return 1;

    // Storing the instructions and data
    struct binary input;
    input.PC = 0;
    memset(input.reg, 0, sizeof(input.reg));
    
    fread(input.inst_mem, 4, INST_MEM_SIZE, file);
    fread(input.data_mem, 1, DATA_MEM_SIZE, file);

    struct node list[128] = {0};
    initialise_heaps(HEAP_SIZE, list);
    
    while (input.inst_mem[input.PC/4] != 0) {
        char opcode[3] = {0};
        char operation[6] = {0};

        get_operation(opcode, operation, &input);   
        
        if (operation[0] == 0) {
            int value = input.inst_mem[input.PC/4];
            printf("Instruction Not Implemented: 0x%08x\n", value);
            register_dump(&input);
            exit(0);
        }

        // Type R operations
        if (strcmp(operation, "add") == 0) {
            add(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "sub") == 0) {
            sub(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "xor") == 0) {
            xor(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "or") == 0) {
            or(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "and") == 0) {
            and(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "sll") == 0) {
            sll(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "srl") == 0) {
            srl(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "sra") == 0) {
            sra(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "slt") == 0) {
            slt(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "sltu") == 0) {
            sltu(input.inst_mem[input.PC/4], &input);
        } 
        // Type I operations
        else if (strcmp(operation, "addi") == 0) {
            addi(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "xori") == 0) {
            xori(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "ori") == 0) {
            ori(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "andi") == 0) {
            andi(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "lb") == 0) {
            lb(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "lh") == 0) {
            lh(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "lw") == 0) {
            lw(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "lbu") == 0) {
            lbu(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "lhu") == 0) {
            lhu(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "slti") == 0) {
            slti(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "sltiu") == 0) {
            sltiu(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "jalr") == 0) {
            jalr(input.inst_mem[input.PC/4], &input);
        } 
        // Type S operations
        else if (strcmp(operation, "sb") == 0) {
            sb(input.inst_mem[input.PC/4], &input, list);
        } else if (strcmp(operation, "sh") == 0) {
            sh(input.inst_mem[input.PC/4], &input, list);
        } else if (strcmp(operation, "sw") == 0) {
            sw(input.inst_mem[input.PC/4], &input, list);
        }
        // Type SB operations
        else if (strcmp(operation, "beq") == 0) {
            beq(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "bne") == 0) {
            bne(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "blt") == 0) {
            blt(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "bltu") == 0) {
            bltu(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "bge") == 0) {
            bge(input.inst_mem[input.PC/4], &input);
        } else if (strcmp(operation, "bgeu") == 0) {
            bgeu(input.inst_mem[input.PC/4], &input);
        } 
        // Type U operations
        else if (strcmp(operation, "lui") == 0) {
            lui(input.inst_mem[input.PC/4], &input);
        } 
        // Type UJ operations
        else if (strcmp(operation, "jal") == 0) {
            jal(input.inst_mem[input.PC/4], &input);
        }
    }
    fclose(file);
    return 0;
}