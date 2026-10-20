#include "MyFunctionsForStrings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>


enum CommandsId
{
    CMD_UNKNOWN = -1,
    CMD_HLT     = 0,
    CMD_PUSH    = 1,
    CMD_ADD     = 2,
    CMD_SUB     = 3,
    CMD_DIV     = 4,
    CMD_OUT     = 5
};

enum CommandsId GetCommandId (const char* name);
void Assembly(FILE* asm_source, FILE* byte_code)


int main()
{
    Assembly()
}

enum CommandsId GetCommandId (const char* name)
{
    if (my_strcmp(name, "HLT") == 0)  return CMD_HLT;
    if (my_strcmp(name, "PUSH") == 0) return CMD_PUSH;
    if (my_strcmp(name, "ADD") == 0)  return CMD_ADD;
    if (my_strcmp(name, "SUB") == 0)  return CMD_SUB;
    if (my_strcmp(name, "DIV") == 0)  return CMD_DIV;
    if (my_strcmp(name, "OUT") == 0)  return CMD_OUT;
    return CMD_UNKNOWN;
}

void Assembly(FILE* asm_source, FILE* byte_code)
{
    char line_buffer [256] = "";
    int line_counter = 0;

    while (fgets(line_buffer, sizeof(line_buffer), asm_source) != NULL)
    {
        line_counter++;
        char cmd_name[32] = "";
        double arg = 0;

        int n_args = sscanf(line_buffer, "%31s %lg", cmd_name, &arg);

        if (n_args <= 0)
            continue;

        enum CommandsId cmd = GetCommandId(cmd_name);
        int has_syntax_error = 0;

        switch (cmd)
        {
            case CMD_PUSH:
                if (n_args == 2)
                    fprintf(byte_code, "%d %d\n", cmd, arg);
                else
                    has_syntax_error = 1;
                break;

            case CMD_HLT:
            case CMD_ADD:
            case CMD_SUB:
            case CMD_DIV:
            case CMD_OUT:
                if (n_args == 1)
                    fprintf(byte_code, "%d\n", cmd);
                else
                    has_syntax_error = 1;
                break;

            default:
                has_syntax_error = 1;
                break;
        }

        if (has_syntax_error)
        {
            printf("Syntax error on line %d\nCompilation failed\n", line_counter);
            break;
        }
    }
}
