#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "protocol.h"

extern int debug_level;
void _log_out_put(char* string, char* type);
void info_print(char *string);
void debug_print(char *string);
void fatal_exit_print(char *string);
#endif