#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

InstructionDecoder::InstructionDecoder(CPU& cpu) : cpu(cpu)
{
    this->reset();

    // Initialize instruction table
    this->opcodes[0x00] = &InstructionDecoder::add_rm8_r8;
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

void InstructionDecoder::add_rm8_r8()
{
}
