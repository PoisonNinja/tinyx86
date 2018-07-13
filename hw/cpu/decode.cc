#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

InstructionDecoder::InstructionDecoder(CPU& cpu) : cpu(cpu)
{
    this->reset();

    // Initialize instruction table
}

InstructionDecoder::~InstructionDecoder()
{
}

void InstructionDecoder::tick()
{
}

void InstructionDecoder::reset()
{
    this->modrm = 0;
}