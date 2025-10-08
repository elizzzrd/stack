#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "read_file.h"


const char * stack_commands[] = 
{
    "HLT", "PUSH", "DUMP", "ADD", "SUB", "MUL", 
    "DIV", "SQRT","OUT", "POPR", "PUSHR"
};

int check_option(char * option)
{
    char option_upper[NMAX] = {};
    to_upper_str(option, option_upper);
    for (int i = 0; i < NCOMMANDS; i++)
    {
        if (strcmp(option_upper, stack_commands[i]) == 0) return i;
    }
    return -1;
}

void to_upper_str(const char * input, char * output)
{
    int i = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        output[i] = (char)toupper((unsigned char) input[i]);
    }
    output[i] = '\0';
    return;
}

size_t get_file_size(const char * filename) 
{
    assert(filename != NULL);
    
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) 
    {
        return (size_t)-1;
    }

    return (size_t)(file_stat.st_size);
}
