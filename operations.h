#ifndef OPERATIONS
#define OPERATIONS

#include "vm_riskxvii.h"
#include "heaps.h"

extern void add(int binary, struct binary* input);
extern void sub(int binary, struct binary* input);
extern void xor(int binary, struct binary* input);
extern void or(int binary, struct binary* input);
extern void and(int binary, struct binary* input);
extern void sll(int binary, struct binary* input);
extern void srl(int binary, struct binary* input);
extern void sra(int binary, struct binary* input);
extern void slt(int binary, struct binary* input);
extern void sltu(int binary, struct binary* input);
extern void addi(int binary, struct binary* input);
extern void xori(int binary, struct binary* input);
extern void ori(int binary, struct binary* input);
extern void andi(int binary, struct binary* input);
extern void slti(int binary, struct binary* input);
extern void sltiu(int binary, struct binary* input);
extern void jalr(int binary, struct binary* input);
extern void lui(int binary, struct binary* input);
extern void lb(int binary, struct binary* input);
extern void lh(int binary, struct binary* input);
extern void lbu(int binary, struct binary* input);
extern void lhu(int binary, struct binary* input);
extern void lw(int binary, struct binary* input);
extern void sb(int binary, struct binary* input, struct node* list);
extern void sh(int binary, struct binary* input, struct node* list);
extern void sw(int binary, struct binary* input, struct node* list);
extern void beq(int binary, struct binary* input);
extern void bne(int binary, struct binary* input);
extern void blt(int binary, struct binary* input);
extern void bltu(int binary, struct binary* input);
extern void bge(int binary, struct binary* input);
extern void bgeu(int binary, struct binary* input);
extern void jal(int binary, struct binary* input);

#endif