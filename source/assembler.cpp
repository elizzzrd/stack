#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "stack.h"
#include "error_stack.h"
#include "read_file.h"



Stack_Err parse_line(const char * line, FILE * file_byte)
{
    char temp_buffer[NMAX] = {};
    strncpy(temp_buffer, line, NMAX);
    temp_buffer[NMAX - 1] = '\0';

    char * comment = strchr(temp_buffer, ';');
    if (comment) (*comment) = '\0';

    char option[NMAX] = {};
    if (sscanf(temp_buffer, "%s", option) != 1) return STACK_OK; //пропускаем пустые строки

    int cmd_index = check_option(option);
    if (cmd_index == -1) 
    {
        fprintf(stderr, "Unknown command in text file\n");
        return STACK_FILE_ERROR;
    }

    if (cmd_index == PUSH)
    {
        int value = 0;
        if (sscanf(temp_buffer + strlen(option), "%d", &value) != 1)
        {
            fprintf(stderr, "Error: push requires an integer\n");
            return STACK_FILE_ERROR;
        }
        fprintf(file_byte, "%d %d\n", cmd_index, value);
    }
    else
    {
        fprintf(file_byte, "%d\n", cmd_index);
    }
    return STACK_OK;
}



Stack_Err assembler(const char * txt_filename, const char * byte_filename)
{
    assert(txt_filename != NULL); 

    FILE * file_txt = fopen(txt_filename, "r");
    if (!file_txt) 
    {
        fprintf(stderr, "Can not open the file %s.\n", txt_filename);
        return STACK_FILE_ERROR;
    }

    FILE * file_byte = fopen(byte_filename, "w");
    if (!file_byte) 
    {
        fprintf(stderr, "Can not open the file %s.\n", txt_filename);
        return STACK_FILE_ERROR;
    }

    char cmd_command[NMAX] = {};
    while (fgets(cmd_command, NMAX, file_txt))
    {
        Stack_Err err = parse_line(cmd_command, file_byte);
        if (err != STACK_OK) 
        {
            fclose(file_txt);
            fclose(file_byte);
            return err;
        }
    }  
    fclose(file_txt);
    fclose(file_byte);
    return STACK_OK;
}


StackElem * load_bytecode(const char * file_byte, size_t * size)
{
    assert(file_byte != NULL);

    FILE * file = fopen(file_byte, "r");
    if (!file) 
    {
        fprintf(stderr, "Can not open the file %s.\n", file_byte);
        return NULL;
    }

    size_t file_size = get_file_size(file_byte);
    StackElem * code = (StackElem *) calloc(file_size + 1, sizeof(StackElem));
    if (!code) 
    { 
        fclose(file); 
        return NULL; 
    }

    int value = 0;
    size_t index = 0;
    while (fscanf(file, "%d", &value) == 1)
    {
        code[index++] = value;
    }
    
    fclose(file);

    *size = index;
    return code;
}


