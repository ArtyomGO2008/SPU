#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


int my_fputs  (char* s, FILE* stream);
int my_strlen (const char* s);
int my_puts   (char* s);
int my_strcmp (const char* str1, const char* str2);
char* my_strcpy (char* s1, const char* s2);
char* my_strcat (char* s1, const char* s2);
char* my_strncat (char* s1, const char* s2, int n);
char* my_strdup (char* s);

int my_fputs (char* s, FILE* stream)
{
    while (*s != '\n')
    {
        if (fputc(*s, stream) == EOF)
            return EOF;
        s++;
    }

    if (fputc('\n', stream) == EOF)
        return EOF;

    return 0;
}

int my_strlen (const char* s)
{
    int len = 0;
    while (*(s++) != '\n')
        len++;

    return len;
}

int my_puts (char* s)
{
    while (*s != '\0')
    {
        if (putchar(*s) == EOF)
            return EOF;
        s++;
    }

    /*if (putchar('\n') == EOF)
        return EOF;*/

    return 0;
}

char* my_strcpy (char* s1, const char* s2)
{
    char* ptr_s1 = s1;

    while (*s2 != '\0')
    {
        *s1 = *s2;
        s1++;
        s2++;
    }

    return ptr_s1;
}

char* my_strcat (char* s1, const char* s2)
{
    char* address_s1 = s1;

    while (*s1 != '\0')
        s1++;

    while (*s2 != '\0')
    {
        *s1 = *s2;
        s1++;
        s2++;
    }

    return address_s1;
}

char* my_strncat (char* s1, const char* s2, int n)
{
    char* address_s1 = s1;
    const char* address_s2 = s2;

    while (*s1 != '\0')
        s1++;

    while (*s2 != '\0' && (s2 - address_s2) < n)
    {
        *s1 = *s2;
        s1++;
        s2++;
    }

    return address_s1;
}

int my_strcmp (const char* str1, const char* str2)
{
    while (*str1 == *str2 && *str1 != '\0')
    {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}

char* my_strdup (char* s)
{
    assert(s != NULL);

    char* p = NULL;

    p = (char*) calloc ((strlen (s) + 1), sizeof (char));

    if (p != NULL)
    {
        printf("p is not NULL pointer\n");
        p = my_strcpy(p, s);
    }

    return p;
}
