#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>

struct ByteCodeBuffer
{
    char* data;
    int size;
    int capacity;
};

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
void ByteCodeBufferInit (ByteCodeBuffer* buf, int max_size);
void ByteCodeBufferFree (ByteCodeBuffer* buf);
int Assembly(const char* asm_source_name, ByteCodeBuffer* buf, const char* byte_code_name);


int main(int argc, char *argv[])
{
    struct ByteCodeBuffer buf_1 = {};

    ByteCodeBufferInit (&buf_1, 1000);

    Assembly("asm_code_v.1", &buf_1, "testing");

    ByteCodeBufferFree (&buf_1);

    return 0;
}

enum CommandsId GetCommandId (const char* name)
{
    if (strcmp(name, "HLT") == 0)  return CMD_HLT;
    if (strcmp(name, "PUSH") == 0) return CMD_PUSH;
    if (strcmp(name, "ADD") == 0)  return CMD_ADD;
    if (strcmp(name, "SUB") == 0)  return CMD_SUB;
    if (strcmp(name, "DIV") == 0)  return CMD_DIV;
    if (strcmp(name, "OUT") == 0)  return CMD_OUT;
    return CMD_UNKNOWN;
}

void ByteCodeBufferInit (ByteCodeBuffer* buf, int max_size)
{
    buf->data = (char*)calloc(max_size, sizeof(char));
    buf->size = 0;
    buf->capacity = max_size;
}

void ByteCodeBufferFree (ByteCodeBuffer* buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int Assembly(const char* asm_source_name, ByteCodeBuffer* buf, const char* byte_code_name)
{
    FILE* asm_source = fopen(asm_source_name, "r");
    if (asm_source == NULL)
    {
        fprintf(stderr, "[ERROR] could not open the file %s: %s\n", asm_source_name, strerror (errno));
        return -1;
    }

    FILE* byte_code = fopen(byte_code_name, "w");
    if (byte_code == NULL)
    {
        fprintf(stderr, "[ERROR] could not open the file %s: %s\n", byte_code_name, strerror (errno));
        return -1;
    }


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
                {
                    //fprintf(byte_code, "%d %lg\n", cmd, arg);
                    *(int*)(buf->data + buf->size) = cmd;
                    buf->size += sizeof(int);
                    *(double*)(buf->data + buf->size) = arg;
                    buf->size += sizeof(double);
                }
                else
                    has_syntax_error = 1;
                break;

            case CMD_HLT:
            case CMD_ADD:
            case CMD_SUB:
            case CMD_DIV:
            case CMD_OUT:
                if (n_args == 1)
                {
                    //fprintf(byte_code, "%d\n", cmd);
                    *(int*)(buf->data + buf->size) = cmd;
                    buf->size += sizeof(int);
                }
                else
                    has_syntax_error = 1;
                break;

            default:
                has_syntax_error = 1;
                break;
        }

        if (has_syntax_error)
        {
            fprintf(stderr, "Syntax error on line %d\nCompilation failed\n", line_counter);
            break;
        }
    }

    for (int i = 0; i < buf->size; i++)
        fprintf(byte_code, "%d", buf->data[i]);

    fclose (asm_source);
    fclose (byte_code);

    return 0;
}
