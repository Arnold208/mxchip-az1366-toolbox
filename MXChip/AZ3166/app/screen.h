/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */
#include <stdint.h>  
#include "ssd1306.h"

#ifndef _SCREEN_H
#define _SCREEN_H

/* Enumration for line on the screen */
typedef enum
{
    L0 = 0,
    L1 = 18,
    L2 = 36,
    L3 = 54
} LINE_NUM;

void screen_print(char* str, LINE_NUM line);
void screen_printn(const char* str, unsigned int str_length, LINE_NUM line);
void screen_printl(char* str, char* str1,LINE_NUM line,LINE_NUM line1);
void draw_youtube_logo(LINE_NUM line0, LINE_NUM line1);
void screen_clear();

#endif // _SCREEN_H