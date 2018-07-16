#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::push_dx()
{
    this->log->trace("push dx");
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DX));
}

void InstructionDecoder::push_edx()
{
    this->log->trace("push edx");
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDX));
}

void InstructionDecoder::push_bx()
{
    this->log->trace("push bx");
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::BX));
}

void InstructionDecoder::push_ebx()
{
    this->log->trace("push ebx");
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EBX));
}

void InstructionDecoder::mov_ax_imm16()
{
    uint16_t val = this->cpu.read_instruction16();
    this->log->trace("mov ax, 0x{:X}", val);
    this->cpu.write_gpreg16(GPRegister16::AX, val);
}

void InstructionDecoder::mov_eax_imm32()
{
    uint32_t val = this->cpu.read_instruction32();
    this->log->trace("mov eax, 0x{:X}", val);
    this->cpu.write_gpreg32(GPRegister32::EAX, val);
}

void InstructionDecoder::mov_sp_imm16()
{
    uint16_t val = this->cpu.read_instruction16();
    this->log->trace("mov sp, 0x{:X}", val);
    this->cpu.write_gpreg16(GPRegister16::SP, val);
}

void InstructionDecoder::mov_esp_imm32()
{
    uint32_t val = this->cpu.read_instruction32();
    this->log->trace("mov esp, 0x{:X}", val);
    this->cpu.write_gpreg32(GPRegister32::ESP, val);
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
