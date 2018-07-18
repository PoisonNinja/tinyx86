#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::and_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->ando(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::inc_bx()
{
    this->cpu.write_gpreg16(
        GPRegister16::BX, this->inc(this->cpu.read_gpreg16(GPRegister16::BX)));
}

void InstructionDecoder::inc_ebx()
{
    this->cpu.write_gpreg32(
        GPRegister32::EBX,
        this->inc(this->cpu.read_gpreg32(GPRegister32::EBX)));
}

void InstructionDecoder::inc_di()
{
    this->cpu.write_gpreg16(
        GPRegister16::DI, this->inc(this->cpu.read_gpreg16(GPRegister16::DI)));
}

void InstructionDecoder::inc_edi()
{
    this->cpu.write_gpreg32(
        GPRegister32::EDI,
        this->inc(this->cpu.read_gpreg32(GPRegister32::EDI)));
}

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

void InstructionDecoder::push_di()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DI));
}

void InstructionDecoder::push_edi()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDI));
}

void InstructionDecoder::pop_dx()
{
    this->cpu.write_gpreg16(GPRegister16::DX, this->cpu.pop16());
}

void InstructionDecoder::pop_edx()
{
    this->cpu.write_gpreg32(GPRegister32::EDX, this->cpu.pop32());
}

void InstructionDecoder::pop_bx()
{
    this->cpu.write_gpreg16(GPRegister16::BX, this->cpu.pop16());
}

void InstructionDecoder::pop_ebx()
{
    this->cpu.write_gpreg32(GPRegister32::EBX, this->cpu.pop32());
}

void InstructionDecoder::pop_di()
{
    this->cpu.write_gpreg16(GPRegister16::DI, this->cpu.pop16());
}

void InstructionDecoder::pop_edi()
{
    this->cpu.write_gpreg32(GPRegister32::EDI, this->cpu.pop32());
}

void InstructionDecoder::jmpnz_ne()
{
    int8_t imm = this->cpu.read_instruction8();
    if (!this->cpu.get_zf()) {
        this->cpu.write_ip(this->cpu.read_ip() + imm);
    }
}

void InstructionDecoder::xchg_r8_rm8()
{
    this->load_modrm();
    uint8_t tmp = this->read_modrm_rm8();
    this->write_modrm_rm8(this->read_modrm_r8());
    this->write_modrm_r8(tmp);
}

void InstructionDecoder::mov_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(this->read_modrm_r8());
}

void InstructionDecoder::mov_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(this->read_modrm_r16());
}

void InstructionDecoder::mov_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(this->read_modrm_r32());
}

void InstructionDecoder::mov_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(this->read_modrm_rm8());
}

void InstructionDecoder::mov_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(this->read_modrm_rm16());
}

void InstructionDecoder::mov_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(this->read_modrm_rm32());
}

void InstructionDecoder::mov_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_bx_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::BX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_ebx_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EBX, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_sp_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::SP, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_esp_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::ESP, this->cpu.read_instruction32());
}

void InstructionDecoder::retn16()
{
    this->cpu.write_ip(this->cpu.pop16());
}

void InstructionDecoder::retn32()
{
    this->cpu.write_eip(this->cpu.pop32());
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

void InstructionDecoder::hlt()
{
    this->cpu.halt();
}