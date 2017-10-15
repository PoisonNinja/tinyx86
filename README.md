# tinyx86
tinyx86 is a simple emulator with the goal of emulating the Intel 80836 and the components necessary to boot up an operating system such as Linux or Windows 95/98.

tinyx86 is released under the BSD 3-Clause license. For the full license details, see the LICENSE file in the root directory

# Details
tinyx86 is designed to emulate the Intel 80836, the first 32-bit protected mode processor from Intel. Currently, tinyx86 implements approximately 20 - 25 opcodes, but a lot of work is going into completing the instruction set. tinyx86 accurately emulates the operations effect on the different registers, including the different quirks inherent in the x86 architecture.

In addition to the processor, tinyx86 aims to implement all the hardware in an actual PC. Future support is planned for:
* VGA framebuffer
* 8254 Programmble Interrupt Timer
* 8259 Programmble Interrupt Controller
* IDE Controller

The current goal of tinyx86 is to be able to boot SeaBIOS, which will eventually allow us to boot actual operating systems.

# Building
## Setting up the environment
Building tinyx86 has only been tested on MacOS (10.12) and Ubuntu (17.04), other operating systems are not officially supported.

You will need a reasonably modern compiler with C11 support. Both Clang (Linux and Mac) and GCC (5 and 6) are known to build tinyx86 successfully, so try to use those if you are having issues with your compiler.

You will also need SDL2 to draw the VGA framebuffer. For Debian derivatives, you can simply run `sudo apt install libsdl2-dev`. For other systems, you will need to manually install SDL2.

## Obtaining the source
It's preferable to obtain the source using git, since that reflects the latest development progress and bugfixes:
```
git clone https://github.com/PoisonNinja/tinyx86.git
```

## Setting up build files
In the tinyx86 directory, create a build directory. In tree builds are not supported to avoid cluttering up the source code tree with object files. Attempting to do so will cause a fatal error.

`cd` to the build directory, and execute `cmake -G <your generator> <path to tinyx86 root>`. Replace `<your generator>` with your preferred build system, such as `make` or `ninja`. Replace `<path to tinyx86 root>` with the path to the root directory of the tinyx86 source.

For example, if my tree looked like: `~/tinyx86` and the build directory was at `~/tinyx86/build`, and I wanted to use Ninja, then I would run `cmake -G Ninja ..`.

CMake should complete without any errors or warnings.

## Building
Simply run your build system. The default target builds a binary called `tinyx86` located in the build directory. At this time, an install target is not provided, so you will have to run it from the build folder.

# Basics
There isn't really much to see currently, but tinyx86 does have some basic functionality implemented.

Upon start, tinyx86 loads SeaBIOS into memory and starts executing it. In its current state, not all opcodes and required hardware support is implemented, so there isn't actualyl anything to see.

Some other useful flags include:
* `--logging-level`: Set the minimum logging level. Higher is more serious
* `-m | --memory`: Set the amount of memory in MiB

By default, tinyx86 prints out only LOG_FATAL messages, but this can be configured using the `--log-level` option as described above.

For more detailed help, run tinyx86 with the `--help` or `-h` argument
