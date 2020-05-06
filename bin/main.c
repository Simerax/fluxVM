#include<stdio.h>
#include"flux_vm.h"

int main(int argc, char** argv)
{
    flux_vm* vm = flux_vm_init();

    /*store value 5 in local variable 0*/
    //flux_vm_istore(vm, 0, 5);
    /*put variable at index 0 onto the stack*/
    //flux_vm_iload(vm, 0);

    /*put integer 10 onto the stack*/
    flux_vm_ipush(vm, 10);
    flux_vm_ipush(vm, 15);

    /*add the two top elements on the stack together (as ints)*/
    flux_vm_iadd(vm);

    /*print the top element of the stack*/
    flux_vm_print(vm);

    flux_vm_free(vm);
    return 0;
}
