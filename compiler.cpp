#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include "MyFunctionsForStrings.cpp"

struct InputFileInformation
{
    int fd;
    int Size;
    char* buffer;
    int LenBuf;
    int NumberOfLines;
    char** index;
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

FILE* OpeningOutputFile (int , char* argv[], const char* name);
int FillIndex (char* pointer, char** index, int NumberOfLines);
struct InputFileInformation CreateIndex (int , char* argv[], const char* FileInputName);
int FileSize (const char* name);
int CountLines (int fd, char* buffer, int SizeOfFile);
enum CommandsId GetCommandId (const char* name);


int main(int argc, char* argv[])
{
    const char* AsmCodeFileName = "asm_code_v.1";
    struct InputFileInformation InputFile = CreateIndex (argc, argv, AsmCodeFileName);
    FILE* byte_code = fopen("testing", "w");

    char cmd_name[32] = {0};
    int arg = 0;

    for (int i = 0; i < InputFile.NumberOfLines; i++)
    {
        int n_args = sscanf(InputFile.index[i],"%s %d", cmd_name, &arg);
        if (n_args <= 0) continue;

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
            printf("Syntax error on line %d\nCompilation failed\n", i + 1);
            break;
        }
    }
}

enum CommandsId GetCommandId (const char* name)
{
    if (my_strcmp(name, "HLT") == 0) return CMD_HLT;
    if (my_strcmp(name, "PUSH") == 0) return CMD_PUSH;
    if (my_strcmp(name, "ADD") == 0) return CMD_ADD;
    if (my_strcmp(name, "SUB") == 0) return CMD_SUB;
    if (my_strcmp(name, "DIV") == 0) return CMD_DIV;
    if (my_strcmp(name, "OUT") == 0) return CMD_OUT;
    return CMD_UNKNOWN;
}

int CountLines (int fd, char* buffer, int SizeOfFile)
{
    assert(buffer != NULL);

    int n_read = read(fd, buffer, SizeOfFile);

    buffer[n_read] = '\n';
    buffer[n_read + 1] = '\0';

    int i = 0;

    char* pointer = buffer;

    while (strchr(pointer, '\n') != NULL)
    {
        pointer = strchr(pointer, '\n') + 1;
        i++;
    }

    return i;
}

int FileSize (const char* name)
{
    struct stat FileInformation;
    stat(name, &FileInformation);

    return (int) FileInformation.st_size;
}

struct InputFileInformation CreateIndex (int , char* argv[], const char* FileInputName)
{
    assert(FileInputName != NULL);

    struct InputFileInformation InputFile = {0};

    InputFile.fd = open(FileInputName, O_RDONLY, 0);
    InputFile.Size = FileSize (FileInputName);

    if (InputFile.fd == -1)
    {
        fprintf (stderr, "%s: no such file in directory %s\n", argv[0], FileInputName);
        exit(EXIT_FAILURE);
    }

    InputFile.buffer = (char*)calloc(InputFile.Size + 1, sizeof(char));

    InputFile.NumberOfLines = CountLines (InputFile.fd, InputFile.buffer, InputFile.Size);

    close(InputFile.fd);

    InputFile.index = (char**) calloc(InputFile.NumberOfLines, sizeof(char*));

    FillIndex(InputFile.buffer, InputFile.index, InputFile.NumberOfLines);

    InputFile.LenBuf = strlen(InputFile.buffer);

    return InputFile;
}

int FillIndex (char* pointer, char** index, int NumberOfLines)
{
    assert(pointer != NULL);
    assert(index != NULL);

    int i = 0;

    while (strchr(pointer, '\n') != NULL)
    {
        assert (i < NumberOfLines);

        index[i] = pointer;

        pointer = strchr(pointer, '\n') + 1;
        i++;
    }

    return 1;
}

FILE* OpeningOutputFile (int , char* argv[], const char* name)
{
    assert (name != NULL);

    FILE* file = fopen(name, "r+");

    if (file == NULL)
    {
        fprintf(stderr, "%s: The file could not be opened. '%s'. Reason: %s (error code %d)\n",
                argv[0],name, strerror(errno), errno);
        exit(EXIT_FAILURE);
    }

    return file;
}

