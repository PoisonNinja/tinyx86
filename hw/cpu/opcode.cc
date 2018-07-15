#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::mov_ax_imm16()
{
    uint16_t val = this->cpu.read_instruction16();
    this->log->trace("mov ax, 0x{:X}", val);
    this->cpu.write_gpreg16(GPRegister::AX, val);
}

void InstructionDecoder::mov_eax_imm32()
{
    uint32_t val = this->cpu.read_instruction32();
    this->log->trace("mov eax, 0x{:X}", val);
    this->cpu.write_gpreg32(GPRegister::AX, val);
}

void InstructionDecoder::mov_sp_imm16()
{
    uint16_t val = this->cpu.read_instruction16();
    this->log->trace("mov sp, 0x{:X}", val);
    this->cpu.write_gpreg16(GPRegister::SP, val);
}

void InstructionDecoder::mov_esp_imm32()
{
    uint32_t val = this->cpu.read_instruction32();
    this->log->trace("mov esp, 0x{:X}", val);
    this->cpu.write_gpreg32(GPRegister::SP, val);
}

void InstructionDecoder::call_rel16()
{
    int16_t imm16 = this->cpu.read_instruction16();
    this->log->trace("call 0x{:X} (rel16)", imm16);
    this->cpu.push16(this->cpu.read_ip());
    this->cpu.write_ip(this->cpu.read_ip() + imm16);
}

void InstructionDecoder::call_rel32()
{
    int32_t imm32 = this->cpu.read_instruction32();
    this->log->trace("call 0x{:X} (rel32)", imm32);
    this->cpu.push32(this->cpu.read_eip());
    this->cpu.write_eip(this->cpu.read_eip() + imm32);
}
