#include <board.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <x86emu.h>

static struct board* board = NULL;

static struct option long_options[] = {
    {"memory", required_argument, 0, 'm'},
    {"help", no_argument, 0, 'h'},
    {NULL, 0, NULL, 0},
};

static void print_usage(char* argv[])
{
    printf("usage: %s [options] [binary_file]\n", argv[0]);
    printf("\n'binary_file' is a flat binary image you would like to run\n");
    printf("\nOptions:\n"
           "-h | --help     Print this help out\n"
           "-m | --memory   Set the memory available to the machine in MiB\n");
}

static void validate_memory(size_t* memory)
{
    if (*memory < X86EMU_MINIMUM_MEMORY) {
        log_warn("Requested memory size is too small, minimum is %zuMB",
                 X86EMU_MINIMUM_MEMORY);
        *memory = X86EMU_MINIMUM_MEMORY;
    } else if (*memory > X86EMU_MAXIMUM_MEMORY) {
        log_warn("Requested memory size is too large, maximum is %zuMB",
                 X86EMU_MAXIMUM_MEMORY);
        *memory = X86EMU_MAXIMUM_MEMORY;
    }
}

int main(int argc, char** argv)
{
    int c, long_index;
    char* end = NULL;
    size_t memory = X86EMU_MINIMUM_MEMORY;
    while ((c = getopt_long(argc, argv, "hm:", long_options, &long_index)) !=
           -1) {
        switch (c) {
            case 'h':
                print_usage(argv);
                exit(0);
            case 'm':
                memory = strtoll(optarg, &end, 0);
                if (*end) {
                    fprintf(stderr,
                            "%s: Option -m requires a numerical argument.\n",
                            argv[0]);
                    exit(1);
                }
                validate_memory(&memory);
                break;
            default:
                exit(1);
        }
    }
    log_info("Memory: %zuMB", memory);
    board = board_create(memory);
    if (!board) {
        exit(1);
    }
    log_trace("Created board");
    board_poweron(board);
    log_trace("Powered on board");
    board_run(board);
    return 0;
}
