#include <board.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <x86emu.h>

static struct board* board = NULL;

static struct option long_options[] = {
    {"binary", required_argument, 0, 'b'},
    {"help", no_argument, 0, 'h'},
    {"memory", required_argument, 0, 'm'},
    {NULL, 0, NULL, 0},
};

static void print_usage(char* argv[])
{
    printf("usage: %s [options]\n", argv[0]);
    printf("\nOptions:\n"
           "-b | --binary   Binary to load\n"
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
    char* binary = NULL;
    size_t memory = X86EMU_MINIMUM_MEMORY;
    while ((c = getopt_long(argc, argv, "hm:", long_options, &long_index)) !=
           -1) {
        switch (c) {
            case 'b':
                binary = optarg;
                break;
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
    if (binary) {
        log_trace("Loading binary %s", binary);
        int fd = open(binary, O_RDONLY);
        if (fd < 0) {
            log_fatal("Could not open binary %s: %s", binary, strerror(errno));
            exit(1);
        }
        size_t binary_size = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        void* buffer = malloc(binary_size);
        size_t bread = 0;
        if ((bread = read(fd, buffer, binary_size)) != binary_size) {
            log_fatal("Failed to read entire binary. Expected %zu, but got %zu",
                      binary_size, bread);
            close(fd);
            exit(1);
        }
        board_load(board, 0, buffer, binary_size);
    }
    board_poweron(board);
    log_trace("Powered on board");
    board_run(board);
    return 0;
}
