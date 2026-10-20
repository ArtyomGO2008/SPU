#include "stack.h"

enum CommandsId
{
    CMD_UNKNOWN = -1,
    CMD_HLT     = 0,
    CMD_PUSH    = 1,
    CMD_ADD     = 2,
    CMD_SUB     = 3,
    CMD_DIV     = 4,
    CMD_OUT     = 5,
    CMD_MUL     = 6
};

struct AsmContext
{
    char* data;
    int size;
    int capacity;
};

struct SPU
{
    double AX;
    double BX;
    double CX;
    double DX;
    stack_t stk;
    char* code;
    int code_size;
    int ip;
};


enum CommandsId GetCommandId (const char* name);
void ByteCodeBufferInit (AsmContext* buf, int max_size);
void ByteCodeBufferFree (AsmContext* buf);
int Assembly(const char* asm_source_name, AsmContext* buf, const char* byte_code_name);

void Add(stack_t* stk);
void Sub(stack_t* stk);
void Div(stack_t* stk);
void Out(stack_t* stk);
int ExecutionSPU (SPU* spu);
int IsEnoughElements (stack_t* stk);
int FileSize (const char* name);
void SPU_Init (SPU* spu, const char* byte_code_name);

