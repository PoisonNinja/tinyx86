#pragma once

#include <stdint.h>

// A struct representing a CPU running in real mode (16-bit addressing)
struct cpu_real {
    uint16_t AX, BX, CX, DX;
    uint16_t SI, DI;
    uint16_t BP, SP;
    uint16_t IP;
    uint16_t CS, DS, SS;
    uint16_t ES, FS, GS;
};
