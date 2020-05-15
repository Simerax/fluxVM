#include "flux_code.h"
#include<string.h> // memcpy
#include<stdlib.h> // malloc
#include<endian.h>
#include "flux_command.h"
#include "flux_instruction.h"
#include"flux_list.h"
#include"flux_log.h"
#include "flux_object.h"


#if __BYTE_ORDER  == __LITTLE_ENDIAN
#define INT32_TO_SYSTEM_ENDIANNESS(x) (x<<24) | ((x&0xf00)<<8) | ((x>>8)&0xff00) | ((x>>24)&0xff)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define INT32_TO_SYSTEM_ENDIANNESS(x) x
#elif __BYTE_ORDER == __PDP_ENDIAN
#error "System Endianness not supported"
#endif


FluxCode* flux_code_init(char* bytes, int length) {

    FluxCode* code = malloc(sizeof(FluxCode));

    FluxCommand** commands;
    int number_of_commands = flux_code_convert_to_flux_commands(bytes, length, &commands);

    code->commands = commands;
    code->number_of_commands = number_of_commands;
    
    return code;
}

void flux_code_free(FluxCode* code) {
    if(code == NULL)
        return;
    
    for(int i = 0; i < code->number_of_commands; i++) {
        if(code->commands[i] != NULL)
            flux_command_free(code->commands[i]);
    }
    free(code);
}

int flux_code_convert_to_flux_commands(char* bytes, int length, FluxCommand*** commands) {

    FluxList* list = flux_list_init(sizeof(FluxCommand*));

    int number_of_commands = 0;


    
    for(int i = 0; i < length; i++)  {
        if (bytes[i] == POP) {
            FluxCommand* pop_command = flux_command_init(POP, NULL, 0);
            flux_list_add(list, pop_command);
            number_of_commands++;
        }
        else if (bytes[i] == IPUSH) {
            int number;
            memcpy(&number, (bytes + i + 1), 4);
            i += 4;
            number = INT32_TO_SYSTEM_ENDIANNESS(number);
            FluxObject* int_obj = flux_object_iinit(number);
            FluxObject** param = malloc(sizeof(FluxObject*));
            param[0] = int_obj;
            FluxCommand* ipush_command = flux_command_init(IPUSH, param, 1);
            flux_list_add(list, ipush_command);
            number_of_commands++;
        }
        else if (bytes[i] == IADD) {
            flux_list_add(list, flux_command_init(IADD, NULL, 0));
            number_of_commands++;
        }
        else if (bytes[i] == LOAD) {
            flux_list_add(list, flux_command_init(LOAD, NULL, 0));
            number_of_commands++;
        }
        else if (bytes[i] == STORE) {
            flux_list_add(list, flux_command_init(STORE, NULL, 0));
            number_of_commands++;
        }
        else if (bytes[i] == PRINT || bytes[i] == INSPECT) {
            FluxInstruction instruction = bytes[i];
            FluxCommand* c = flux_command_init(instruction, NULL, 0);
            flux_list_add(list, c);
            number_of_commands++;
        }
        else if (bytes[i] == ITOD) {
            int number;
            memcpy(&number, (bytes + i + 1), 4);
            i += 4;
            number = INT32_TO_SYSTEM_ENDIANNESS(number);
            FluxObject* itod_obj = flux_object_iinit(number);
            FluxObject** param = malloc(sizeof(FluxObject*));
            param[0] = itod_obj;
            FluxCommand* ipush_command = flux_command_init(ITOD, param, 1);
            flux_list_add(list, ipush_command);
            number_of_commands++;
        }
        else if (bytes[i] == JSR || bytes[i] == JE || bytes[i] == JL || bytes[i] == JLE || bytes[i] == JGE) {
            FluxInstruction instruction = bytes[i];
            int number = read_integer((bytes + i + 1));
            i += 4;
            FluxObject* obj = flux_object_iinit(number);
            FluxObject** param = malloc(sizeof(FluxObject*));
            param[0] = obj;
            FluxCommand* jmp = flux_command_init(instruction, param, 1);
            flux_list_add(list, jmp);
            number_of_commands++;
        }
        else if (bytes[i] == CMP) {
            flux_list_add(list, flux_command_init(CMP, NULL, 0));
            number_of_commands++;
        }
        else {
            FLUX_WLOG("Unknown Command '%d' in bytecode at byte position %d -- Command is being ignored", bytes[i], i);
        }
    }


    *commands = (FluxCommand**)flux_list_to_array(list);
    
    return number_of_commands;
}


int read_integer(char* buffer) {
    int number;
    memcpy(&number, buffer, 4);
    number = INT32_TO_SYSTEM_ENDIANNESS(number);
    return number;
}
