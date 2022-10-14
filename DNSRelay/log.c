#include "log.h"

void _log_out_put(char *string, char *type)
{
    time_t timer = time(NULL);
    char *time_string = ctime(&timer);
    while (*time_string != ' ')
        time_string++;
    time_string++;
    time_string[strlen(time_string) - 6] = '\0';
    struct timeval tv;
    gettimeofday(&tv,NULL);
    char ms[5];
    sprintf(ms, ":%03ld", tv.tv_usec/1000);
    strcat(time_string, ms);
    if(strcmp(type, "info") == 0)
        printf("\033[;32m%s [%s] : %s\033[0m", time_string, type, string);
    else if(strcmp(type, "debug") == 0)
        printf("\033[;33m%s [%s] : %s\033[0m", time_string, type, string);
    else if(strcmp(type, "fatal") == 0)
        printf("\033[;31m%s [%s] : %s\033[0m", time_string, type, string);
    printf("\033[;37m\n\033[0m");
    
}

void info_print(char *string)
{
    if (debug_level >= 1)
        _log_out_put(string, "info");
}

void debug_print(char *string)
{
    if (debug_level >= 2)
        _log_out_put(string, "debug");
}

void fatal_exit_print(char *string)
{
    _log_out_put(string, "fatal");
    printf("EXITING...\n");
    exit(0);
}