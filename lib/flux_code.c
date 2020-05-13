#include "flux_code.h"
#include<string.h> // memcpy
#include<stdlib.h> // malloc
#include"flux_list.h"


//#if __BTYE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define INT32_TO_SYSTEM_ENDIANNESS(x) (x<<24) | ((x&0xf00)<<8) | ((x>>8)&0xff00) | ((x>>24)&0xff)
//#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
//#define INT32_TO_SYSTEM_ENDIANNESS(x) x
//#else 
//#error System Endianness not supported
//#endif 

flux_code* flux_code_init(char* bytes, int length) {

    flux_code* code = malloc(sizeof(flux_code));

    FluxCommand** commands;
    int number_of_commands = flux_code_convert_to_flux_commands(bytes, length, &commands);

    code->commands = commands;
    code->number_of_commands = number_of_commands;
    
    return code;
}

void flux_code_free(flux_code* code) {
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
        if (bytes[i] == IPUSH) {
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
        if (bytes[i] == IADD) {
            flux_list_add(list, flux_command_init(IADD, NULL, 0));
            number_of_commands++;
        }
        if (bytes[i] == LOAD) {
            flux_list_add(list, flux_command_init(LOAD, NULL, 0));
            number_of_commands++;
        }
        if (bytes[i] == STORE) {
            flux_list_add(list, flux_command_init(STORE, NULL, 0));
            number_of_commands++;
        }
        if (bytes[i] == PRINT) {
            FluxCommand* c = flux_command_init(PRINT, NULL, 0);
            flux_list_add(list, c);
            number_of_commands++;
        }
    }


    *commands = (FluxCommand**)flux_list_to_array(list);
    
    return number_of_commands;
}
