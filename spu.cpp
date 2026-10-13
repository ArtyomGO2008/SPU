#include <stdio.h>

int main()
{
    int cmd = 0;
    int arg = 0;
    FILE* byte_code = fopen("testing","r");
    while (fscanf(byte_code, "%d %d", &cmd, &arg) != EOF)
    {
        switch (cmd)
        {
            case (0):
                return 0;
                break;

            case (1):
                StackPush(&stk1, arg);
                break;

            case(2):
                Add();
                break;

            case(3):
                Sub();
                break;

            case(4):
                Div();
                break;

            case(5):
                Out();
                break;
        }
    }
}

void Add(void)
{
    int val_1 = 0;
    int val_2 = 0;
    StackPop(&stk1, &val_1);
    StackPop(&stk1, &val_2);
    StackPush(&stk1, val_1 + val_2);
}

void Sub(void)
{
    int val_1 = 0;
    int val_2 = 0;
    StackPop(&stk1, &val_1);
    StackPop(&stk1, &val_2);
    StackPush(&stk1, val_2 - val_1);
}

void Div(void)
{
    int val_1 = 0;
    int val_2 = 0;
    StackPop(&stk1, &val_1);
    Stackpop(&stk1, &val_2);
    if (val_1 != 0)
        StackPush(&stk1, val_2/val_1);
}

void Out(void)
{
    int val_1 = 0;
    StackPop(&stk1, &val_1);
    printf("%d",val_1);
}
