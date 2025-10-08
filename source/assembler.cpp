#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#include "stack.h"
#include "errors.h"
#include "read_file.h"
#include "spu.h"



Spu_Err parse_line(const char * line, FILE * file_byte)
{
    Spu_Err errors = SPU_OK;

    if (!line || !file_byte) errors |= SPU_FILE_ERROR;

    char temp_buffer[NMAX] = {};
    strncpy(temp_buffer, line, NMAX);
    temp_buffer[NMAX - 1] = '\0';

    char * comment = strchr(temp_buffer, ';'); // убираем комментарии
    if (comment) (*comment) = '\0';

    char option[NMAX] = {};
    if (sscanf(temp_buffer, "%s", option) != 1) errors |= SPU_OK; //пропускаем пустые строки

    int cmd_index = check_option(option);
    if (cmd_index == -1) 
    {
        log_message("Unknown command in text file\n", __FILE__, __LINE__);
        errors |= SPU_UNKNOWN_COMMAND;
    }

    if (cmd_index == PUSH)
    {
        int value = 0;
        if (sscanf(temp_buffer + strlen(option), "%d", &value) != 1)
        {
            log_message("Error: push requires an integer\n", __FILE__, __LINE__);
            errors |= SPU_FILE_ERROR;
        }
        fprintf(file_byte, "%d %d\n", cmd_index, value);
    }
    else if (cmd_index == POPR || cmd_index == PUSHR)
    {
        int value = 0;
        char reg_buffer[NMAX] = {};
        int reg_num = 0;
        if ((sscanf(temp_buffer + strlen(option), "%s", reg_buffer) != 1) && (reg_num = check_register(reg_buffer)) == 0) errors |= SPU_INVALID_COMMAND;
        else
        {
            fprintf(file_byte, "%d %d\n", cmd_index, reg_num);
        }
    }
    else
    {
        fprintf(file_byte, "%d\n", cmd_index);
    }
    return errors;
}


int check_register(const char * reg_buffer)
{
    int reg_num = (toupper(reg_buffer[1]) - 'A') + 1;
    if (strlen(reg_buffer) != 3 && (reg_num < 1 || reg_num > 16)) return 0;
    else return reg_num;
}

Spu_Err assembler(const char * txt_filename, const char * byte_filename) // функция compile??
{
    Spu_Err errors = SPU_OK;
    assert(txt_filename != NULL); 
    assert(byte_filename != NULL); 

    FILE * file_txt = fopen(txt_filename, "r");
    if (!file_txt) 
    {
        fprintf(log_file, "Can not open the file %s.\n", txt_filename); // log_message?? другая реализация с переменным числом аргументов
        errors |= SPU_FILE_ERROR;
    }

    FILE * file_byte = fopen(byte_filename, "w");
    if (!file_byte) 
    {
        fprintf(log_file, "Can not open the file %s.\n", txt_filename); //
        errors |= SPU_FILE_ERROR;
    }

    char cmd_command[NMAX] = {};
    while (fgets(cmd_command, NMAX, file_txt))
    {
        Spu_Err err = parse_line(cmd_command, file_byte);
        if (err != SPU_OK) 
        {
            fclose(file_txt);
            fclose(file_byte);
            errors |= err;
            return errors;
        }
    }  
    fclose(file_txt);
    fclose(file_byte);
    return errors;
}


int * load_bytecode(const char * file_byte, size_t * size)
{
    assert(file_byte != NULL);
    assert(size != NULL);

    FILE * file = fopen(file_byte, "r");
    if (!file) 
    {
        fprintf(log_file, "Can not open the file %s.\n", file_byte);
        return NULL;
    }

    size_t file_size = get_file_size(file_byte);
    int * code = (int *) calloc(file_size + 1, sizeof(StackElem)); // какого размера массив code 
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


