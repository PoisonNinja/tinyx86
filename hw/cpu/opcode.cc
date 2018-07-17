#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::push_dx()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DX));
}

void InstructionDecoder::push_edx()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDX));
}

void InstructionDecoder::push_bx()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::BX));
}

void InstructionDecoder::push_ebx()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EBX));
}

void InstructionDecoder::mov_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_sp_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::SP, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_esp_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::ESP, this->cpu.read_instruction32());
}

void InstructionDecoder::call_rel16()
{
    /*
     * Order matters! Do not try to move the imm fetch into the write_ip
     * call because then we will push the wrong address onto the stack to
     * return to.
     */
    uint16_t imm = this->cpu.read_instruction16();
    this->cpu.push16(this->cpu.read_ip());
    this->cpu.write_ip(this->cpu.read_ip() + imm);
}

void InstructionDecoder::call_rel32()
{
    uint32_t imm = this->cpu.read_instruction32();
    this->cpu.push32(this->cpu.read_eip());
    this->cpu.write_eip(this->cpu.read_eip() + imm);
}
