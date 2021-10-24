#include "memory.hpp"

int main(int argc, char *argv[]) {
    char* mem_type = (char*) malloc(5);
    strcpy(mem_type, "RAM\0");
    long long mem_size = 1024;
    long long launch_cnt = 10;
    bool max_buffer = false;
    if (Process_parameters(argc, argv, mem_type, mem_size,
                launch_cnt, max_buffer) == 1)
    {
        return 1;
    }
    printf("--- arguments of main(): --- \n");
    printf("mem_type = %s \n", mem_type);
    printf("mem_size = %lld bytes \n", mem_size);
    printf("launch_cnt = %lld \n", launch_cnt);
    printf("max_buffer = %d \n", max_buffer);

    srand(time(0));
    Tests_handler(mem_type, launch_cnt, mem_size, max_buffer);

    free(mem_type);
    return 0;
}
