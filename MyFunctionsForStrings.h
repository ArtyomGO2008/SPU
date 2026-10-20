#pragma once
#include <stdio.h>

int my_fputs  (char* s, FILE* stream);
int my_strlen (const char* s);
int my_puts   (char* s);
int my_strcmp (const char* str1, const char* str2);
char* my_strcpy (char* s1, const char* s2);
char* my_strcat (char* s1, const char* s2);
char* my_strncat (char* s1, const char* s2, int n);
char* my_strdup (char* s);

#pragma once
