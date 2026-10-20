#include "VirtualMachine.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "stack.h"


int main()
{
    SPU spu_1 = {};

    SPU_Init (&spu_1, "testing");

    printf("code_size = %d, ip = %d, code ptr = %p\n", spu_1.code_size, spu_1.ip, spu_1.code );

    ExecutionSPU (&spu_1);

    return 0;
}

void Add(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;

    if (IsEnoughElements(stk))
    {
        StackPop(stk, &val_1);
        StackPop(stk, &val_2);
    }

    StackPush(stk, val_1 + val_2);
}

void Sub(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;

    if (IsEnoughElements(stk))
    {
        StackPop(stk, &val_1);
        StackPop(stk, &val_2);
    }

    StackPush(stk, val_2 - val_1);
}

void Div(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;

    if (IsEnoughElements(stk))
    {
        StackPop(stk, &val_1);
        StackPop(stk, &val_2);
    }

    if (val_1 == 0)
    {
        fprintf(stderr, "[ERROR] Division by Zero\n        Execution aborted\n");
        exit(1);
    }

    StackPush(stk, val_2/val_1);
}

void Mul(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;

    if (IsEnoughElements(stk))
    {
        StackPop(stk, &val_1);
        StackPop(stk, &val_2);
    }

    StackPush(stk, val_1*val_2);
}

void Out(stack_t* stk)
{
    double val_1 = 0;
    StackPop(stk, &val_1);
    printf("%lg",val_1);
}

void SPU_Init (SPU* spu, const char* byte_code_name)
{
    STACK_INIT(spu->stk, 5);
    spu->ip = 0;

    FILE* byte_code = fopen(byte_code_name,"rb");

    if (byte_code == NULL)
    {
        fprintf(stderr, "[ERROR] could not open the file \"%s\": %s\n", byte_code_name, strerror (errno));
        exit(1);
    }

    spu->code_size = FileSize (byte_code_name);

    spu->code = (char*)malloc(spu->code_size);

    fread(spu->code, sizeof(char), spu->code_size, byte_code);

    fclose(byte_code);
}

int FileSize (const char* name)
{
    struct stat FileInformation;
    stat(name, &FileInformation);

    return (int) FileInformation.st_size;
}

int IsEnoughElements (stack_t* stk)
{
    if (stk->size < 2)
    {
        fprintf(stderr, "[ERROR] not enough elements in Stack for the command\nExecution aborted\n");
        exit(1);
    }

    return 1;
}

int ExecutionSPU (SPU* spu)
{
    int cmd = 0;
    int arg = 0;

    while (spu->ip < spu->code_size)
    {
        cmd = *(int*)(spu->code + spu->ip);
        spu->ip += sizeof(int);

        printf("cmd = %d\n", cmd);

        switch (cmd)
        {
            case (CMD_HLT):
                return 0;

            case (CMD_PUSH):
                //fscanf(byte_code, "%d", &arg);
                arg = *(double*)(spu->code + spu->ip);
                printf("arg of push = %d\n", arg);

                spu->ip += sizeof(double);
                StackPush(&spu->stk, arg);
                StackDump("log_stack", 0, &spu->stk);
                break;

            case(CMD_ADD):
                Add(&spu->stk);
                StackDump("log_stack", 0, &spu->stk);
                break;

            case(CMD_SUB):
                Sub(&spu->stk);
                StackDump("log_stack", 0, &spu->stk);
                break;

            case(CMD_DIV):
                Div(&spu->stk);
                StackDump("log_stack", 0, &spu->stk);
                break;

            case(CMD_OUT):
                Out(&spu->stk);
                StackDump("log_stack", 0, &spu->stk);
                break;

            case(CMD_MUL):
                Mul(&spu->stk);
                StackDump("log_stack", 0, &spu->stk);
                break;
        }
        //printf("After commands: %d\n", stk1.size);
    }
}
