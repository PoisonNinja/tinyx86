#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::add_rm8_r8()
{
}

void InstructionDecoder::mov_sp_imm16()
{
    this->log->trace("mov_sp_imm16");
    this->cpu.write_gpreg16(GPRegister::SP, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_esp_imm32()
{
    this->log->trace("mov_esp_imm32");
    this->cpu.write_gpreg32(GPRegister::SP, this->cpu.read_instruction32());
}
