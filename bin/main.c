#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include"flux_vm.h"

int main(int argc, char** argv)
{
    if(argc < 2) {
        puts("No Arguments specified. You need to specify at least a .fvm file");
        return 1;
    }

    char* filename = argv[argc - 1];

    FILE* f = fopen(filename, "r");
    if(f == NULL) {
        printf("Failed to open file %s Error: %d", filename, errno);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long f_length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* fvm_filecontent = malloc(f_length);
    fread(fvm_filecontent, 1, f_length, f);
    fclose(f);


    FluxCode* code = flux_code_init(fvm_filecontent, f_length);
    FluxVM* vm = flux_vm_init();
    free(fvm_filecontent);

    FluxVMError error = flux_vm_execute(vm, code);
    if(error.type != flux_vm_error_type_no_error) {
        puts("Error while executing bytecode\n");
        switch(error.type) {
            case flux_vm_error_type_uncaught_exception: printf("Uncaught Exception at Position %d", error.position); break;
            default: puts("Unknown Error"); break;
        }
        puts("\n");
    }
    flux_code_free(code);
    flux_vm_free(vm);
    return 0;
}
