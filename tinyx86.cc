#include <cxxopts.h>
#include <hw/board.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <tinyx86.h>
#include <string>

constexpr size_t minimum_ram = 128;
constexpr size_t maximum_ram = 1024;

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
    Board board(memory);
    stdout->info("Board created");
    board.run();
    return 0;
}
