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
#include "assembler.h"


static int labels[MAX_LABELS];
static int code_buffer[MAX_CODE_SIZE];
static size_t code_size = 0;

void init_labels(void) 
{
    for (int i = 0; i < MAX_LABELS; i++)
        labels[i] = -1;
}


Spu_Err parse_argument(const char *arg, int * value) 
{
    Spu_Err errors = SPU_OK;
    if (!arg || !*arg) return SPU_INVALID_COMMAND;

    if (is_number(arg)) 
    {
        *value = atoi(arg);
        return errors;
    }
    else if (arg[0] == 'r' && isalpha((unsigned char)arg[1])) 
    {
        int reg_num = (toupper(arg[1]) - 'A') + 1;
        if (strlen(arg) != 3 && (reg_num < 1 || reg_num > 16)) 
        {
            log_message("Invalid register name", __FILE__, __LINE__);
            return SPU_INVALID_COMMAND;
        }
        *value = reg_num;
        return errors;
    } 
    else if (is_label(arg)) 
    {
        int id = atoi(arg + 1);
        if (id < 0 || id >= MAX_LABELS) 
        {
            log_message("Label ID out of range", __FILE__, __LINE__);
            return SPU_INVALID_COMMAND;
        }
        if (labels[id] == -1) {
            log_message("Label used but not defined", __FILE__, __LINE__);
            return SPU_INVALID_COMMAND;
        }
        *value = labels[id];
        return errors;
    }

    log_message("Unrecognized argument type", __FILE__, __LINE__);
    return SPU_INVALID_COMMAND;
}

/*
Spu_Err parse_line(const char * line)
{
    Spu_Err errors = SPU_OK;

    if (!line) errors |= SPU_FILE_ERROR;

    char temp_buffer[NMAX] = {};
    strncpy(temp_buffer, line, NMAX);
    temp_buffer[NMAX - 1] = '\0';

    char * comment = strchr(temp_buffer, ';'); 
    if (comment) (*comment) = '\0'; // убираем комментарии

    char option[NMAX] = {};
    if (sscanf(temp_buffer, "%s", option) != 1)  return SPU_OK; // пустая строка
    
    if (is_label(option)) return SPU_OK; // определение метки

    int cmd_index = check_option(option);
    if (cmd_index == -1) 
    {
        log_message("Unknown command in text file\n", __FILE__, __LINE__);
        errors |= SPU_UNKNOWN_COMMAND;
    }

    code_buffer[code_size++] = cmd_index;

    char arg[NMAX] = {};
    if (sscanf(temp_buffer + strlen(option), "%s", arg) == 1)
    {
        if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1]))) 
        {
            code_buffer[code_size++] = atoi(arg);
        }
        else if (arg[0] == 'r' && isalpha(arg[1])) 
        {
            int reg_num = check_register(arg);
            code_buffer[code_size++] = reg_num;
        }
        else if (is_label(arg)) // метка
        {
            int id = atoi(arg + 1);
            if (id >= 0 && id < MAX_LABELS)
            {
                if (labels[id] == -1) /// ЧТО ДЕЛАТЬ ПРИ ТАКОЙ ОШИБКЕ
                {
                    log_message("Label not defined", __FILE__, __LINE__);
                    code_buffer[code_size++] = 0;
                }
                else
                    code_buffer[code_size++] = labels[id];
            }
        }
        else
        {
            log_message("Invalid argument", __FILE__, __LINE__);
        }
    }
    return errors;
}
*/


Spu_Err first_pass(const char * txt_filename) 
{
    Spu_Err errors = SPU_OK;
    assert(txt_filename != NULL); 
    
    FILE * file_txt = fopen(txt_filename, "r");
    if (!file_txt) 
    {
        fprintf(log_file, "Can not open the file %s.\n", txt_filename); // log_message?? другая реализация с переменным числом аргументов
        errors |= SPU_FILE_ERROR;
    }

    init_labels();
    char cmd_command[NMAX] = {};
    size_t ip = 0;

    while (fgets(cmd_command, NMAX, file_txt))
    {
        char option[NMAX] = {};
        if (sscanf(cmd_command, "%s", option) != 1) continue;

        if (is_label(option))
        {
            int id = atoi(option + 1);
            if (id >= 0 && id < MAX_LABELS)
                labels[id] = (int)ip;
            else
                log_message("Label index out of range", __FILE__, __LINE__);
            continue;
        }

        int cmd = check_option(option);
        if (cmd == -1) 
        {
            log_message("Unknown command in text file\n", __FILE__, __LINE__);
            errors |= SPU_UNKNOWN_COMMAND;
        }


        if (cmd == PUSH)
        {
            ip += 2; 
        }
        else if (cmd == POP)
        {
            char arg[NMAX] = {};
            if (sscanf(cmd_command + strlen(option), "%s", arg) == 1)
                ip += 2; 
            else
                ip += 1; 
        }
        else if (cmd >= JB && cmd <= JMP)
        {
            ip += 2; 
        }
        else
        {
            ip += 1; 
        }
    }  
    fclose(file_txt);
    return errors;
}


Spu_Err second_pass(const char *txt_filename) 
{
    Spu_Err errors = SPU_OK;
    assert(txt_filename);

    FILE *file = fopen(txt_filename, "r");
    if (!file) {
        log_message("Cannot open input file (second pass)", __FILE__, __LINE__);
        return SPU_FILE_ERROR;
    }

    code_size = 0;
    char cmd_command[NMAX] = {};

    while (fgets(cmd_command, NMAX, file)) 
    {
        char option[NMAX] = {};
        if (sscanf(cmd_command, "%s", option) != 1) continue;

        char * comment = strchr(cmd_command, ';'); 
        if (comment) (*comment) = '\0'; // убираем комментарии

        if (is_label(option))
            continue; // метки игнорируются во втором проходе

        int cmd = check_option(option);
        if (cmd == -1) 
        {
            log_message("Unknown command", __FILE__, __LINE__);
            errors |= SPU_UNKNOWN_COMMAND;
            continue;
        }

        code_buffer[code_size++] = cmd;

        char arg[NMAX] = {};
        if (sscanf(cmd_command + strlen(option), "%s", arg) == 1) {
            int value = 0;
            errors |= parse_argument(arg, &value);
            code_buffer[code_size++] = value;;
        }
    }

    fclose(file);
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
    int * code = (int *) calloc(file_size + 1, sizeof(StackElem));  
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


Spu_Err assembler(const char * txt_filename, const char * byte_filename) 
{
    assert(txt_filename);
    assert(byte_filename);

    Spu_Err errors = SPU_OK;

    errors |= first_pass(txt_filename);
    errors |= second_pass(txt_filename);

    if (errors != SPU_OK) {
        log_message("Assembler failed during passes", __FILE__, __LINE__);
        return errors;
    }

    FILE *out = fopen(byte_filename, "w");
    if (!out) {
        log_message("Cannot open output file", __FILE__, __LINE__);
        return SPU_FILE_ERROR;
    }

    for (size_t i = 0; i < code_size; i++)
        fprintf(out, "%d\n", code_buffer[i]);

    fclose(out);
    return errors;
}