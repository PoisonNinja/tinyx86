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

In it's current state, tinyx86 can run a simple 'Hello, World!' program. This early in the development stage, tinyx86 does sacrifice some accuracy for the sake of ease of use. For example:
* The CPU starts executing instructions from 0x0
* The VGA buffer is located at 0x8000 instead of 0xB8000 to make it easier to access the buffer without changing the DS register
* Upon CPU reset, the CS limit is set to 0xFFFF

# Building
## Setting up the environment
Building tinyx86 has only been tested on MacOS (10.12) and Ubuntu (17.04), other operating systems are not officially supported.

tinyx86 at this time does not require any dependencies beyond a C11-compliant compiler and NASM (optional if you are not compiling your own binaries). Both Clang (Linux and Mac) and GCC (5 and 6) are known to build tinyx86 successfully, so try to use those if you are having issues with your compiler.

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
There isn't really much to see currently, but tinyx86 does have some basic functionality. A prebuilt binary called `hello` is located in the test/ folder. The source for that binary is `hello.asm`, and the disassembly of the binary made using ndisasm is called `hello.disasm`.

To boot up tinyx86 with the `hello` binary, simply run: `tinyx86 --binary <path to binary>`. You can try running your own binaries, but keep in mind a few points:
* tinyx86 is very much a work in progress, and most likely your custom binary will fail with an `Invalid Opcode` message
* The binary loaded must be a flat binary (for NASM, `-f bin`)
* The binary can not be larger than a segment

Some other useful flags include:
* `--logging-level`: Set the minimum logging level. Higher is more serious
* `-m | --memory`: Set the amount of memory in MiB

By default, tinyx86 prints out log messages greater than LOG_INFO, but this can be configured using the `--log-level` option as described above.

For more detailed help, run tinyx86 with the `--help` or `-h` argument
