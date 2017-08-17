#include <cpu/cpu.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct cpu* bsp = NULL;

static size_t memory = X86EMU_MINIMUM_MEMORY;

static void validate_memory(size_t* memory)
{
    if (*memory < X86EMU_MINIMUM_MEMORY)
        *memory = X86EMU_MINIMUM_MEMORY;
    else if (*memory > X86EMU_MAXIMUM_MEMORY)
        *memory = X86EMU_MAXIMUM_MEMORY;
}

int main(int argc, char** argv)
{
    int c;
    char* end = NULL;
    while ((c = getopt(argc, argv, "m:")) != -1) {
        switch (c) {
            case 'm':
                memory = strtoll(optarg, &end, 0);
                if (*end) {
                    fprintf(stderr,
                            "Option -m requires a numerical argument.\n");
                    exit(1);
                }
                validate_memory(&memory);
                break;
            case '?':
                if (optopt == 'm')
                    fprintf(stderr, "Option -%c requires an argument.\n",
                            optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                exit(1);
        }
    }
    log_info("Memory: %zu\n", memory);
    bsp = cpu_create();
    cpu_destroy(bsp);
    return 0;
}
