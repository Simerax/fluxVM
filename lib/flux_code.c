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

    flux_command** commands;
    int number_of_commands = flux_code_convert_to_flux_commands(bytes, length, &commands);

    code->commands = commands;
    code->number_of_commands = number_of_commands;
    
    return code;
}


int flux_code_convert_to_flux_commands(char* bytes, int length, flux_command*** commands) {

    flux_list* list = flux_list_init(sizeof(flux_command*));

    int number_of_commands = 0;


    
    for(int i = 0; i < length; i++)  {
        if (bytes[i] == POP) {
            flux_command* pop_command = flux_command_init(POP, NULL, 0);
            flux_list_add(list, pop_command);
            number_of_commands++;
        }
        if (bytes[i] == IPUSH) {
            int number;
            memcpy(&number, bytes, 4);
            i += 4;
            number = INT32_TO_SYSTEM_ENDIANNESS(number);
            flux_object* int_obj = flux_object_iinit(number);
            flux_object** param = malloc(sizeof(flux_object*));
            param[0] = int_obj;
            flux_command* ipush_command = flux_command_init(IPUSH, param, 1);
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
            flux_command* c = flux_command_init(PRINT, NULL, 0);
            flux_list_add(list, c);
            number_of_commands++;
        }
    }


    *commands = (flux_command**)flux_list_to_array(list);
    
    return number_of_commands;
}
