#include<stdio.h>
#include"flux_vm.h"

int main(int argc, char** argv)
{
    FluxVM* vm = flux_vm_init();

    /*store value 5 in variable 0*/
    flux_vm_ipush(vm, 5);
    flux_vm_ipush(vm, 0);
    flux_vm_store(vm);

    /*put variable 0 onto the stack*/
    flux_vm_load(vm, 0);

    /*put integer 10 onto the stack*/
    flux_vm_ipush(vm, 10);

    /*integer addition of the two stack elements*/
    flux_vm_iadd(vm);

    /*store the result of add in variable at index 1*/
    flux_vm_ipush(vm, 1);
    flux_vm_store(vm);


    /*load variable from index 1 and print it*/
    flux_vm_load(vm, 1);
    flux_vm_print(vm);


    /* load variable 1 convert it to double*/
    flux_vm_load(vm, 1);
    flux_vm_itod(vm);

    /* store new value in local var 1 */
    flux_vm_ipush(vm, 1);
    flux_vm_store(vm);

    flux_vm_load(vm, 1);
    flux_vm_print(vm);



    flux_vm_free(vm);
    return 0;
}
