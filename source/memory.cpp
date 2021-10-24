#include "memory.hpp"

int Process_parameters(int argc, char *argv[], char* mem_type,
        long long &mem_size, long long &launch_cnt,
        bool &max_buffer)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp("-m", argv[i]) == 0 ||
                strcmp("--memory-type", argv[i]) == 0)
        {
            i++;
            if (strcmp("RAM", argv[i]) == 0 ||
                    strcmp("SSD", argv[i]) == 0 ||
                    strcmp("flash", argv[i]) == 0)
            {
                strcpy(mem_type, argv[i]);
            }
            else {
                printf("Error in arguments of main(): incorrect value for --memory-type \n");
                return 1;
            }
        }
        else if (strcmp("-b", argv[i]) == 0 ||
                strcmp("--block-size", argv[i]) == 0)
        {
            i++;
            size_t len = strlen(argv[i]);
            int coefficient = 1;
            if (argv[i][len-1] == 'b') {
                if (argv[i][len-2] == 'K')
                    coefficient = 1024;
                else if (argv[i][len-2] == 'M')
                    coefficient = 1024 * 1024;
                else {
                    printf("Error in arguments of main(): incorrect description of unit of measure byte \n");
                    return 1;
                }
                strncpy(argv[i], argv[i], len - 2);
            }
            mem_size = atoll(argv[i]) * coefficient;
        }
        else if (strcmp("-l", argv[i]) == 0 ||
                strcmp("--launch-count", argv[i]) == 0)
        {
            i++;
            launch_cnt = atoll(argv[i]);
            if (launch_cnt == 0) {
                printf("Error in arguments of main(): incorrect value for --launch-count \n");
                return 1;
            }
        }
        else if (strcmp("--maximum-buffer", argv[i]) == 0)
        {
            max_buffer = true;
        }
        else {
            printf("Error in arguments of main(): incorrect value (ind = %d) \n", i);
            return 1;
        }
    }

    return 0;
}

int Test_RAM(long long mem_size, bool max_buffer,
        double &time_write, double &time_read)
{
    uint8_t* arr1 = (uint8_t*) malloc(mem_size);
    long long i;
    for (i = 0; i < mem_size; i++) {
        arr1[i] = rand() % 256;
    }
    uint8_t* arr2 = (uint8_t*) malloc(mem_size);
    clock_t start, stop;
    if (max_buffer) {
        start = clock();
        memcpy(arr2, arr1, mem_size);
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            arr2[i] = arr1[i];
        }
        stop = clock();
    }
    time_write = ((double)(stop - start)) / CLOCKS_PER_SEC;
    time_read = time_write;
    free(arr1);
    free(arr2);
    return 0;
}

int Test_storage_device(char* mem_type, long long mem_size, bool max_buffer,
        double &time_write, double &time_read)
{
    uint8_t* arr = (uint8_t*) malloc(mem_size);
    long long i;
    for (i = 0; i < mem_size; i++) {
        arr[i] = rand() % 256;
    }
    FILE *fp;
    if (strcmp("SSD", mem_type) == 0) {
        if ((fp = fopen("test_SSD.txt", "w")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_SSD.txt \n");
            return 1;
        }
    }
    else if (strcmp("flash", mem_type) == 0) {
        if ((fp = fopen("/mnt/f/test_flash.txt", "w")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_flash.txt in USB \n");
            return 1;
        }
    }
    clock_t start, stop;
    if (max_buffer) {
        start = clock();
        fwrite(arr, mem_size, 1, fp);
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            fprintf(fp, "%hhu ", arr[i]);
        }
        stop = clock();
    }
    time_write = ((double)(stop - start)) / CLOCKS_PER_SEC;
    fclose(fp);
    if (strcmp("SSD", mem_type) == 0) {
        if ((fp = fopen("test_SSD.txt", "r")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_SSD.txt \n");
            return 1;
        }
    }
    else if (strcmp("flash", mem_type) == 0) {
        if ((fp = fopen("/mnt/f/test_flash.txt", "r")) == NULL) {
            printf("Error in Test_storage_device(): can't open file test_flash.txt in USB \n");
            return 1;
        }
    }
    if (max_buffer) {
        start = clock();
        fread(arr, mem_size, 1, fp);
        stop = clock();
    }
    else {
        start = clock();
        for (i = 0; i < mem_size; i++) {
            fscanf(fp, "%hhu", &arr[i]);
        }
        stop = clock();
    }
    time_read = ((double)(stop - start)) / CLOCKS_PER_SEC;

    free(arr);
    fclose(fp);
    return 0;
}

int Write_to_csv(char* mem_type, long long launch_cnt,
        long long mem_size, bool max_buffer,
        double time_write[], double avg_time_write,
        double abs_error_write, double rel_error_write,
        double time_read[], double avg_time_read,
        double abs_error_read, double rel_error_read)
{
    FILE *fout;
    if ((fout = fopen("../data/output.csv", "a")) == NULL) {
        printf("Error in Write_to_csv(): can't open output.csv \n");
        return 1;
    }
    for (long long i = 0; i < launch_cnt; i++) {
        fprintf(fout, "%s;", mem_type);
        fprintf(fout, "%lld;", mem_size);
        fprintf(fout, "uint8_t;");
        if (max_buffer)
            fprintf(fout, "%lld;", mem_size);
        else
            fprintf(fout, "1;");
        fprintf(fout, "%lld;", i+1);
        fprintf(fout, "clock();");
        fprintf(fout, "%e;", time_write[i]);
        fprintf(fout, "%e;", avg_time_write);
        fprintf(fout, "%e;", (double)mem_size / avg_time_write * 1e6);
        fprintf(fout, "%e;", abs_error_write);
        fprintf(fout, "%e%%;", rel_error_write);
        fprintf(fout, "%e;", time_read[i]);
        fprintf(fout, "%e;", avg_time_read);
        fprintf(fout, "%e;", (double)mem_size / avg_time_read * 1e6);
        fprintf(fout, "%e;", abs_error_read);
        fprintf(fout, "%e%%;", rel_error_read);
        fprintf(fout, "\n");
    }
    return 0;
}

int Tests_handler(char* mem_type, long long launch_cnt,
        long long mem_size, bool max_buffer)
{
    double summand1_write = 0, summand2_write = 0;
    double summand1_read = 0, summand2_read = 0;
    double time_write[launch_cnt];
    double time_read[launch_cnt];
    for (long long i = 0; i < launch_cnt; i++) {
        if (strncmp("RAM", mem_type, 3) == 0) {
            if (Test_RAM(mem_size, max_buffer, time_write[i], time_read[i]) == 1)
                return 1;
        }
        else {
            if (Test_storage_device(mem_type, mem_size, max_buffer,
                        time_write[i], time_read[i]) == 1)
                return 1;
        }
        summand1_write += time_write[i] * time_write[i];
        summand2_write += time_write[i];
        summand1_read += time_read[i] * time_read[i];
        summand2_read += time_read[i];
    }
    summand1_write /= launch_cnt;
    summand2_write /= launch_cnt;
    double avg_time_write = summand2_write;
    summand2_write *= summand2_write;
    double dispersion_write = summand1_write - summand2_write;
    double abs_error_write = sqrt(dispersion_write);
    double rel_error_write = dispersion_write / avg_time_write * 100;
    summand1_read /= launch_cnt;
    summand2_read /= launch_cnt;
    double avg_time_read = summand2_read;
    summand2_read *= summand2_read;
    double dispersion_read = summand1_read - summand2_read;
    double abs_error_read = sqrt(dispersion_read);
    double rel_error_read = dispersion_read / avg_time_read * 100;

    Write_to_csv(mem_type, launch_cnt, mem_size, max_buffer,
            time_write, avg_time_write,
            abs_error_write, rel_error_write,
            time_read, avg_time_read,
            abs_error_read, rel_error_read);
    return 0;
}
