#ifndef _COLORED_LOG_H_

#define _COLORED_LOG_H_

#pragma region IncludedHeaders

#include <stdio.h>

#pragma endregion

#pragma region Defines

// Available colors
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_BLUE 34
#define COLOR_WHITE 37

// Colord print macro
#define COLORED_LOG(color_code, format, ...) printf("\033[%dm[+] " format "\n\033[0m", color_code, ##__VA_ARGS__)

#pragma endregion

#endif