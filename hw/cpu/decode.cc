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
    uint8_t opcode = cpu.read_instruction8();
}

void InstructionDecoder::reset()
{
    this->modrm = 0;
}
