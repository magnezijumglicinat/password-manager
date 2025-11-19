

#include <stdio.h>
#include <windows.h>
#include "typewriter.h"

void typewrite(float ms, const char* text, ...)
{
    char buf[512];

    va_list args;
    va_start(args, text);
    vsnprintf(buf, sizeof(buf), text, args);
    va_end(args);

    for (int i = 0; buf[i] != '\0'; ++i) {
        putchar(buf[i]);
        fflush(stdout);
        Sleep(ms);
    }
}