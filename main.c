#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <hw/board.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyx86.h>
#include <unistd.h>

static struct board* board = NULL;

#define GETOPT_BINARY 0x1000
#define GETOPT_LOG_LEVEL 0x1001

static struct option long_options[] = {
    {"binary", required_argument, 0, GETOPT_BINARY},
    {"log-level", required_argument, 0, GETOPT_LOG_LEVEL},
    {"help", no_argument, 0, 'h'},
    {"memory", required_argument, 0, 'm'},
    {"version", no_argument, 0, 'v'},
    {NULL, 0, NULL, 0},
};

static void print_usage(char* argv[])
{
    printf("usage: %s [options]\n", argv[0]);
    printf(
        "\nOptions:\n"
        "--binary            Binary to load\n"
        "-h | --help         Print this help out\n"
        "--log-level         Logging level. Higher is more serious\n"
        "-m | --memory       Set the memory available to the machine in MiB\n"
        "-v | --version      Display the version number\n");
}

static void print_version()
{
    printf("%s %d.%d.%d\n", TINYX86_VERSION_NAME, TINYX86_VERSION_MAJOR,
           TINYX86_VERSION_MINOR, TINYX86_VERSION_PATCH);
}

static void validate_memory(size_t* memory)
{
    if (*memory < TINYX86_MINIMUM_MEMORY) {
        log_warn("Requested memory size is too small, minimum is %zuMB",
                 TINYX86_MINIMUM_MEMORY);
        *memory = TINYX86_MINIMUM_MEMORY;
    } else if (*memory > TINYX86_MAXIMUM_MEMORY) {
        log_warn("Requested memory size is too large, maximum is %zuMB",
                 TINYX86_MAXIMUM_MEMORY);
        *memory = TINYX86_MAXIMUM_MEMORY;
    }
}

static void validate_log_level(int* level)
{
    if (*level < LOG_TRACE) {
        log_warn("Requested logging level is too low, minimum is %d",
                 LOG_TRACE);
        *level = LOG_TRACE;
    } else if (*level > LOG_FATAL) {
        log_warn("Requested logging level is too high, maximum is %d",
                 LOG_FATAL);
        *level = LOG_FATAL;
    }
}

int main(int argc, char** argv)
{
    int c, long_index;
    char* end = NULL;
    char* binary = NULL;
    int log_level = LOG_FATAL;
    size_t memory = TINYX86_MINIMUM_MEMORY;
    while ((c = getopt_long(argc, argv, "hm:v", long_options, &long_index)) !=
           -1) {
        switch (c) {
            case GETOPT_BINARY:
                binary = optarg;
                break;
            case GETOPT_LOG_LEVEL:
                log_level = atoi(optarg);
                validate_log_level(&log_level);
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
            case 'v':
                print_version();
                exit(0);
            default:
                exit(1);
        }
    }
    log_set_level(log_level);
    log_info("Memory: %zuMB", memory);
    board = board_create(memory);
    if (!board) {
        exit(1);
    }
    log_debug("Created board");
    if (binary) {
        log_debug("Loading binary %s", binary);
        tinyx86_file_t file = tinyx86_file_open(binary, "r");
        if (!file) {
            log_fatal("Could not open binary %s: %s", binary, strerror(errno));
            tinyx86_exit(1);
        }
        size_t binary_size = tinyx86_file_size(file);
        void* buffer = malloc(binary_size);
        size_t bread = 0;
        if ((bread = tinyx86_file_read(file, buffer, binary_size)) !=
            binary_size) {
            log_fatal("Failed to read entire binary. Expected %zu, but got %zu",
                      binary_size, bread);
            tinyx86_file_close(file);
            tinyx86_exit(1);
        }
        tinyx86_file_close(file);
        board_load(board, buffer, binary_size);
    }
    board_poweron(board);
    log_debug("Powered on board");
    board_run(board);
    return 0;
}
