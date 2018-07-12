#include <cxxopts.h>
#include <hw/board.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <tinyx86.h>
#include <string>

constexpr size_t minimum_ram = 128;
constexpr size_t maximum_ram = 1024;

// static struct board* board = NULL;

// #define GETOPT_LOG_LEVEL 0x1001

// static struct option long_options[] = {
//     {"log-level", required_argument, 0, GETOPT_LOG_LEVEL},
//     {"help", no_argument, 0, 'h'},
//     {"memory", required_argument, 0, 'm'},
//     {"version", no_argument, 0, 'v'},
//     {NULL, 0, NULL, 0},
// };

// static void print_usage(char* argv[])
// {
//     printf("usage: %s [options]\n", argv[0]);
//     printf(
//         "\nOptions:\n"
//         "-h | --help         Print this help out\n"
//         "--log-level         Logging level. Higher is more serious\n"
//         "-m | --memory       Set the memory available to the machine in
//         MiB\n"
//         "-v | --version      Display the version number\n");
// }

// static void print_version()
// {
//     printf("%s %d.%d.%d\n", TINYX86_VERSION_NAME, TINYX86_VERSION_MAJOR,
//            TINYX86_VERSION_MINOR, TINYX86_VERSION_PATCH);
// }

// static void validate_memory(size_t* memory)
// {
//     if (*memory < TINYX86_MINIMUM_MEMORY) {
//         log_warn("Requested memory size is too small, minimum is %zuMB",
//                  TINYX86_MINIMUM_MEMORY);
//         *memory = TINYX86_MINIMUM_MEMORY;
//     } else if (*memory > TINYX86_MAXIMUM_MEMORY) {
//         log_warn("Requested memory size is too large, maximum is %zuMB",
//                  TINYX86_MAXIMUM_MEMORY);
//         *memory = TINYX86_MAXIMUM_MEMORY;
//     }
// }

// static void validate_log_level(int* level)
// {
//     if (*level < LOG_TRACE) {
//         log_warn("Requested logging level is too low, minimum is %d",
//                  LOG_TRACE);
//         *level = LOG_TRACE;
//     } else if (*level > LOG_FATAL) {
//         log_warn("Requested logging level is too high, maximum is %d",
//                  LOG_FATAL);
//         *level = LOG_FATAL;
//     }
// }

int main(int argc, const char** argv)
{
    auto stdout = spdlog::stderr_color_mt("stdout");
    stdout->set_level(spdlog::level::info);

    size_t memory = minimum_ram;

    cxxopts::Options options("tinyx86", "A tiny and simple x86 emulator");
    options.add_options()("h,help", "Print help")(
        "log-level", "Minimum log level to be printed", cxxopts::value<int>())(
        "m,memory", "Memory allocated for the board", cxxopts::value<size_t>());

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cerr << options.help({"", "Group"}) << std::endl;
            return 0;
        }

        if (result.count("memory")) {
            if (result["memory"].as<size_t>() < minimum_ram) {
                memory = minimum_ram;
            } else if (result["memory"].as<size_t>() > maximum_ram) {
                memory = maximum_ram;
            } else {
                memory = result["memory"].as<size_t>();
            }
        }

        if (result.count("log-level")) {
            if (result["log-level"].as<int>() <
                    static_cast<int>(spdlog::level::level_enum::trace) ||
                result["log-level"].as<int>() >
                    static_cast<int>(spdlog::level::level_enum::off)) {
                std::cerr << "Invalid log level set. Valid range is from "
                          << static_cast<int>(spdlog::level::level_enum::trace)
                          << " to "
                          << static_cast<int>(spdlog::level::level_enum::off)
                          << "\n";
            } else {
                stdout->set_level(static_cast<spdlog::level::level_enum>(
                    result["log-level"].as<int>()));
            }
        }
    } catch (const cxxopts::OptionException& e) {
        std::cerr << "Error parsing option: " << e.what() << "\n\n";
        std::cerr << options.help({"", "Group"}) << std::endl;
        return 0;
    }
    stdout->info("Memory: {} MiB", memory);
    //     board = board_create(memory);
    //     if (!board) {
    //         exit(1);
    //     }
    //     log_debug("Created board");
    //     log_debug("Launching window...");
    //     window_create(720, 400);
    //     board_poweron(board);
    //     log_debug("Powered on board");
    //     board_run(board);
    //     return 0;
}
