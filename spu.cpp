#include <stdio.h>
#include "stack.h"

void Add(stack_t* stk);
void Sub(stack_t* stk);
void Div(stack_t* stk);
void Out(stack_t* stk);

int main()
{
    int cmd = 0;
    int arg = 0;
    stack_t stk1 = {};
    STACK_INIT(stk1, 5);
    FILE* byte_code = fopen("testing","r");
    while (fscanf(byte_code, "%d", &cmd) != EOF)
    {
        switch (cmd)
        {
            case (0):
                return 0;

            case (1):
                fscanf(byte_code, "%d", &arg);
                StackPush(&stk1, arg);
                StackDump("log_stack", 0, &stk1);
                break;

            case(2):
                Add(&stk1);
                StackDump("log_stack", 0, &stk1);
                break;

            case(3):
                Sub(&stk1);
                StackDump("log_stack", 0, &stk1);
                break;

            case(4):
                Div(&stk1);
                StackDump("log_stack", 0, &stk1);
                break;

            case(5):
                Out(&stk1);
                StackDump("log_stack", 0, &stk1);
                break;
        }
        //printf("After commands: %d\n", stk1.size);
    }
}

void Add(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;
    StackPop(stk, &val_1);
    StackPop(stk, &val_2);
    StackPush(stk, val_1 + val_2);
}

void Sub(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;
    StackPop(stk, &val_1);
    StackPop(stk, &val_2);
    StackPush(stk, val_2 - val_1);
}

void Div(stack_t* stk)
{
    double val_1 = 0;
    double val_2 = 0;
    StackPop(stk, &val_1);
    StackPop(stk, &val_2);
    if (val_1 != 0)
        StackPush(stk, val_2/val_1);
}

void Out(stack_t* stk)
{
    double val_1 = 0;
    StackPop(stk, &val_1);
    printf("%lg",val_1);
}
