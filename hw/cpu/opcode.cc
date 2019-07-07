#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::add_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->add(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::add_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->add(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::add_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->add(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::add_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->add(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::add_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->add(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::add_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->add(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::add_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->add(this->cpu.read_gpreg8(GPRegister8::AL),
                                     this->cpu.read_instruction8()));
}

void InstructionDecoder::add_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX,
                            this->add(this->cpu.read_gpreg16(GPRegister16::AX),
                                      this->cpu.read_instruction16()));
}

void InstructionDecoder::add_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX,
                            this->add(this->cpu.read_gpreg32(GPRegister32::EAX),
                                      this->cpu.read_instruction32()));
}

void InstructionDecoder::push_es16()
{
    this->cpu.push16(this->cpu.read_sgreg(SGRegister::ES));
}

void InstructionDecoder::push_es32()
{
    this->cpu.push32(this->cpu.read_sgreg(SGRegister::ES));
}

void InstructionDecoder::pop_es16()
{
    this->cpu.write_sgreg(SGRegister::ES, this->cpu.pop16());
}

void InstructionDecoder::pop_es32()
{
    this->cpu.write_sgreg(SGRegister::ES, this->cpu.pop32() & 0xFFFF);
}

void InstructionDecoder::or_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->do_or(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::or_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->do_or(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::or_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->do_or(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::or_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->do_or(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::or_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->do_or(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::or_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->do_or(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::or_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->do_or(this->cpu.read_gpreg8(GPRegister8::AL),
                                       this->cpu.read_instruction8()));
}

void InstructionDecoder::or_ax_imm16()
{
    this->cpu.write_gpreg16(
        GPRegister16::AX, this->do_or(this->cpu.read_gpreg16(GPRegister16::AX),
                                      this->cpu.read_instruction16()));
}

void InstructionDecoder::or_eax_imm32()
{
    this->cpu.write_gpreg32(
        GPRegister32::EAX,
        this->do_or(this->cpu.read_gpreg32(GPRegister32::EAX),
                    this->cpu.read_instruction32()));
}

void InstructionDecoder::push_cs16()
{
    this->cpu.push16(this->cpu.read_sgreg(SGRegister::CS));
}

void InstructionDecoder::push_cs32()
{
    this->cpu.push32(this->cpu.read_sgreg(SGRegister::CS));
}

void InstructionDecoder::adc_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->adc(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::adc_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->adc(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::adc_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->adc(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::adc_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->adc(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::adc_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->adc(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::adc_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->adc(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::adc_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->adc(this->cpu.read_gpreg8(GPRegister8::AL),
                                     this->cpu.read_instruction8()));
}

void InstructionDecoder::adc_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX,
                            this->adc(this->cpu.read_gpreg16(GPRegister16::AX),
                                      this->cpu.read_instruction16()));
}

void InstructionDecoder::adc_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX,
                            this->adc(this->cpu.read_gpreg32(GPRegister32::EAX),
                                      this->cpu.read_instruction32()));
}

void InstructionDecoder::push_ss16()
{
    this->cpu.push16(this->cpu.read_sgreg(SGRegister::SS));
}

void InstructionDecoder::push_ss32()
{
    this->cpu.push32(this->cpu.read_sgreg(SGRegister::SS));
}

void InstructionDecoder::pop_ss16()
{
    this->cpu.write_sgreg(SGRegister::SS, this->cpu.pop16());
}

void InstructionDecoder::pop_ss32()
{
    this->cpu.write_sgreg(SGRegister::SS, this->cpu.pop32() & 0xFFFF);
}

void InstructionDecoder::sbb_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->sbb(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::sbb_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->sbb(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::sbb_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->sbb(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::sbb_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->sbb(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::sbb_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->sbb(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::sbb_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->sbb(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::sbb_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->sbb(this->cpu.read_gpreg8(GPRegister8::AL),
                                     this->cpu.read_instruction8()));
}

void InstructionDecoder::sbb_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX,
                            this->sbb(this->cpu.read_gpreg16(GPRegister16::AX),
                                      this->cpu.read_instruction16()));
}

void InstructionDecoder::sbb_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX,
                            this->sbb(this->cpu.read_gpreg32(GPRegister32::EAX),
                                      this->cpu.read_instruction32()));
}

void InstructionDecoder::push_ds16()
{
    this->cpu.push16(this->cpu.read_sgreg(SGRegister::DS));
}

void InstructionDecoder::push_ds32()
{
    this->cpu.push32(this->cpu.read_sgreg(SGRegister::DS));
}

void InstructionDecoder::pop_ds16()
{
    this->cpu.write_sgreg(SGRegister::DS, this->cpu.pop16());
}

void InstructionDecoder::pop_ds32()
{
    this->cpu.write_sgreg(SGRegister::DS, this->cpu.pop32() & 0xFFFF);
}

void InstructionDecoder::and_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->do_and(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::and_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->do_and(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::and_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->do_and(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::and_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->do_and(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::and_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->do_and(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::and_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->do_and(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::and_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->do_and(this->cpu.read_gpreg8(GPRegister8::AL),
                                        this->cpu.read_instruction8()));
}

void InstructionDecoder::and_ax_imm16()
{
    this->cpu.write_gpreg16(
        GPRegister16::AX, this->do_and(this->cpu.read_gpreg16(GPRegister16::AX),
                                       this->cpu.read_instruction16()));
}

void InstructionDecoder::and_eax_imm32()
{
    this->cpu.write_gpreg32(
        GPRegister32::EAX,
        this->do_and(this->cpu.read_gpreg32(GPRegister32::EAX),
                     this->cpu.read_instruction32()));
}

void InstructionDecoder::daa_al()
{
    this->daa();
}

void InstructionDecoder::sub_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->sub(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::sub_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->sub(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::sub_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->sub(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::sub_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->sub(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::sub_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->sub(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::sub_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->sub(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::sub_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->sub(this->cpu.read_gpreg8(GPRegister8::AL),
                                     this->cpu.read_instruction8()));
}

void InstructionDecoder::sub_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX,
                            this->sub(this->cpu.read_gpreg16(GPRegister16::AX),
                                      this->cpu.read_instruction16()));
}

void InstructionDecoder::sub_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX,
                            this->sub(this->cpu.read_gpreg32(GPRegister32::EAX),
                                      this->cpu.read_instruction32()));
}

void InstructionDecoder::das_al()
{
    this->das();
}

void InstructionDecoder::xor_rm8_r8()
{
    this->load_modrm();
    this->write_modrm_rm8(
        this->do_xor(this->read_modrm_rm8(), this->read_modrm_r8()));
}

void InstructionDecoder::xor_rm16_r16()
{
    this->load_modrm();
    this->write_modrm_rm16(
        this->do_xor(this->read_modrm_rm16(), this->read_modrm_r16()));
}

void InstructionDecoder::xor_rm32_r32()
{
    this->load_modrm();
    this->write_modrm_rm32(
        this->do_xor(this->read_modrm_rm32(), this->read_modrm_r32()));
}

void InstructionDecoder::xor_r8_rm8()
{
    this->load_modrm();
    this->write_modrm_r8(
        this->do_xor(this->read_modrm_r8(), this->read_modrm_rm8()));
}

void InstructionDecoder::xor_r16_rm16()
{
    this->load_modrm();
    this->write_modrm_r16(
        this->do_xor(this->read_modrm_r16(), this->read_modrm_rm16()));
}

void InstructionDecoder::xor_r32_rm32()
{
    this->load_modrm();
    this->write_modrm_r32(
        this->do_xor(this->read_modrm_r32(), this->read_modrm_rm32()));
}

void InstructionDecoder::xor_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->do_xor(this->cpu.read_gpreg8(GPRegister8::AL),
                                        this->cpu.read_instruction8()));
}

void InstructionDecoder::xor_ax_imm16()
{
    this->cpu.write_gpreg16(
        GPRegister16::AX, this->do_xor(this->cpu.read_gpreg16(GPRegister16::AX),
                                       this->cpu.read_instruction16()));
}

void InstructionDecoder::xor_eax_imm32()
{
    this->cpu.write_gpreg32(
        GPRegister32::EAX,
        this->do_xor(this->cpu.read_gpreg32(GPRegister32::EAX),
                     this->cpu.read_instruction32()));
}

void InstructionDecoder::aaa_al_ah()
{
    this->aaa();
}

void InstructionDecoder::cmp_rm8_r8()
{
    this->load_modrm();
    this->cmp(this->read_modrm_rm8(), this->read_modrm_r8());
}

void InstructionDecoder::cmp_rm16_r16()
{
    this->load_modrm();
    this->cmp(this->read_modrm_rm16(), this->read_modrm_r16());
}

void InstructionDecoder::cmp_rm32_r32()
{
    this->load_modrm();
    this->cmp(this->read_modrm_rm32(), this->read_modrm_r32());
}

void InstructionDecoder::cmp_r8_rm8()
{
    this->load_modrm();
    this->cmp(this->read_modrm_r8(), this->read_modrm_rm8());
}

void InstructionDecoder::cmp_r16_rm16()
{
    this->load_modrm();
    this->cmp(this->read_modrm_r16(), this->read_modrm_rm16());
}

void InstructionDecoder::cmp_r32_rm32()
{
    this->load_modrm();
    this->cmp(this->read_modrm_r32(), this->read_modrm_rm32());
}

void InstructionDecoder::cmp_al_imm8()
{
    this->cmp(this->cpu.read_gpreg8(GPRegister8::AL),
              this->cpu.read_instruction8());
}

void InstructionDecoder::cmp_ax_imm16()
{
    this->cmp(this->cpu.read_gpreg16(GPRegister16::AX),
              this->cpu.read_instruction16());
}

void InstructionDecoder::cmp_eax_imm32()
{
    this->cmp(this->cpu.read_gpreg32(GPRegister32::EAX),
              this->cpu.read_instruction32());
}

void InstructionDecoder::aas_al_ah()
{
    this->aas();
}

void InstructionDecoder::inc_ax()
{
    this->cpu.write_gpreg16(
        GPRegister16::AX, this->inc(this->cpu.read_gpreg16(GPRegister16::AX)));
}

void InstructionDecoder::inc_eax()
{
    this->cpu.write_gpreg32(
        GPRegister32::EAX,
        this->inc(this->cpu.read_gpreg32(GPRegister32::EAX)));
}

void InstructionDecoder::inc_cx()
{
    this->cpu.write_gpreg16(
        GPRegister16::CX, this->inc(this->cpu.read_gpreg16(GPRegister16::CX)));
}

void InstructionDecoder::inc_ecx()
{
    this->cpu.write_gpreg32(
        GPRegister32::ECX,
        this->inc(this->cpu.read_gpreg32(GPRegister32::ECX)));
}

void InstructionDecoder::inc_dx()
{
    this->cpu.write_gpreg16(
        GPRegister16::DX, this->inc(this->cpu.read_gpreg16(GPRegister16::DX)));
}

void InstructionDecoder::inc_edx()
{
    this->cpu.write_gpreg32(
        GPRegister32::EDX,
        this->inc(this->cpu.read_gpreg32(GPRegister32::EDX)));
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

void InstructionDecoder::inc_sp()
{
    this->cpu.write_gpreg16(
        GPRegister16::SP, this->inc(this->cpu.read_gpreg16(GPRegister16::SP)));
}

void InstructionDecoder::inc_esp()
{
    this->cpu.write_gpreg32(
        GPRegister32::ESP,
        this->inc(this->cpu.read_gpreg32(GPRegister32::ESP)));
}

void InstructionDecoder::inc_bp()
{
    this->cpu.write_gpreg16(
        GPRegister16::BP, this->inc(this->cpu.read_gpreg16(GPRegister16::BP)));
}

void InstructionDecoder::inc_ebp()
{
    this->cpu.write_gpreg32(
        GPRegister32::EBP,
        this->inc(this->cpu.read_gpreg32(GPRegister32::EBP)));
}

void InstructionDecoder::inc_si()
{
    this->cpu.write_gpreg16(
        GPRegister16::SI, this->inc(this->cpu.read_gpreg16(GPRegister16::SI)));
}

void InstructionDecoder::inc_esi()
{
    this->cpu.write_gpreg32(
        GPRegister32::ESI,
        this->inc(this->cpu.read_gpreg32(GPRegister32::ESI)));
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

void InstructionDecoder::dec_ax()
{
    this->cpu.write_gpreg16(
        GPRegister16::AX, this->dec(this->cpu.read_gpreg16(GPRegister16::AX)));
}

void InstructionDecoder::dec_eax()
{
    this->cpu.write_gpreg32(
        GPRegister32::EAX,
        this->dec(this->cpu.read_gpreg32(GPRegister32::EAX)));
}

void InstructionDecoder::dec_cx()
{
    this->cpu.write_gpreg16(
        GPRegister16::CX, this->dec(this->cpu.read_gpreg16(GPRegister16::CX)));
}

void InstructionDecoder::dec_ecx()
{
    this->cpu.write_gpreg32(
        GPRegister32::ECX,
        this->dec(this->cpu.read_gpreg32(GPRegister32::ECX)));
}

void InstructionDecoder::dec_dx()
{
    this->cpu.write_gpreg16(
        GPRegister16::DX, this->dec(this->cpu.read_gpreg16(GPRegister16::DX)));
}

void InstructionDecoder::dec_edx()
{
    this->cpu.write_gpreg32(
        GPRegister32::EDX,
        this->dec(this->cpu.read_gpreg32(GPRegister32::EDX)));
}

void InstructionDecoder::dec_bx()
{
    this->cpu.write_gpreg16(
        GPRegister16::BX, this->dec(this->cpu.read_gpreg16(GPRegister16::BX)));
}

void InstructionDecoder::dec_ebx()
{
    this->cpu.write_gpreg32(
        GPRegister32::EBX,
        this->dec(this->cpu.read_gpreg32(GPRegister32::EBX)));
}

void InstructionDecoder::dec_sp()
{
    this->cpu.write_gpreg16(
        GPRegister16::SP, this->dec(this->cpu.read_gpreg16(GPRegister16::SP)));
}

void InstructionDecoder::dec_esp()
{
    this->cpu.write_gpreg32(
        GPRegister32::ESP,
        this->dec(this->cpu.read_gpreg32(GPRegister32::ESP)));
}

void InstructionDecoder::dec_bp()
{
    this->cpu.write_gpreg16(
        GPRegister16::BP, this->dec(this->cpu.read_gpreg16(GPRegister16::BP)));
}

void InstructionDecoder::dec_ebp()
{
    this->cpu.write_gpreg32(
        GPRegister32::EBP,
        this->dec(this->cpu.read_gpreg32(GPRegister32::EBP)));
}

void InstructionDecoder::dec_si()
{
    this->cpu.write_gpreg16(
        GPRegister16::SI, this->dec(this->cpu.read_gpreg16(GPRegister16::SI)));
}

void InstructionDecoder::dec_esi()
{
    this->cpu.write_gpreg32(
        GPRegister32::ESI,
        this->dec(this->cpu.read_gpreg32(GPRegister32::ESI)));
}

void InstructionDecoder::dec_di()
{
    this->cpu.write_gpreg16(
        GPRegister16::DI, this->dec(this->cpu.read_gpreg16(GPRegister16::DI)));
}

void InstructionDecoder::dec_edi()
{
    this->cpu.write_gpreg32(
        GPRegister32::EDI,
        this->dec(this->cpu.read_gpreg32(GPRegister32::EDI)));
}

void InstructionDecoder::push_ax()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::AX));
}

void InstructionDecoder::push_eax()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EAX));
}

void InstructionDecoder::push_cx()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::CX));
}

void InstructionDecoder::push_ecx()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::ECX));
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

void InstructionDecoder::push_sp()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::SP));
}

void InstructionDecoder::push_esp()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::ESP));
}

void InstructionDecoder::push_bp()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::BP));
}

void InstructionDecoder::push_ebp()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EBP));
}

void InstructionDecoder::push_si()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::SI));
}

void InstructionDecoder::push_esi()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::ESI));
}

void InstructionDecoder::push_di()
{
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DI));
}

void InstructionDecoder::push_edi()
{
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDI));
}
void InstructionDecoder::pop_ax()
{
    this->cpu.write_gpreg16(GPRegister16::AX, this->cpu.pop16());
}

void InstructionDecoder::pop_eax()
{
    this->cpu.write_gpreg32(GPRegister32::EAX, this->cpu.pop32());
}

void InstructionDecoder::pop_cx()
{
    this->cpu.write_gpreg16(GPRegister16::CX, this->cpu.pop16());
}

void InstructionDecoder::pop_ecx()
{
    this->cpu.write_gpreg32(GPRegister32::ECX, this->cpu.pop32());
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

void InstructionDecoder::pop_sp()
{
    this->cpu.write_gpreg16(GPRegister16::SP, this->cpu.pop16());
}

void InstructionDecoder::pop_esp()
{
    this->cpu.write_gpreg32(GPRegister32::ESP, this->cpu.pop32());
}

void InstructionDecoder::pop_bp()
{
    this->cpu.write_gpreg16(GPRegister16::BP, this->cpu.pop16());
}

void InstructionDecoder::pop_ebp()
{
    this->cpu.write_gpreg32(GPRegister32::EBP, this->cpu.pop32());
}

void InstructionDecoder::pop_si()
{
    this->cpu.write_gpreg16(GPRegister16::SI, this->cpu.pop16());
}

void InstructionDecoder::pop_esi()
{
    this->cpu.write_gpreg32(GPRegister32::ESI, this->cpu.pop32());
}

void InstructionDecoder::pop_di()
{
    this->cpu.write_gpreg16(GPRegister16::DI, this->cpu.pop16());
}

void InstructionDecoder::pop_edi()
{
    this->cpu.write_gpreg32(GPRegister32::EDI, this->cpu.pop32());
}

void InstructionDecoder::pusha16()
{
    // TODO: Throw page fault if we're unable to push all registers
    uint16_t old_sp = this->cpu.read_gpreg16(GPRegister16::SP);
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::AX));
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::CX));
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DX));
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::BX));
    this->cpu.push16(old_sp);
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::BP));
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::SI));
    this->cpu.push16(this->cpu.read_gpreg16(GPRegister16::DI));
}

void InstructionDecoder::pusha32()
{
    uint32_t old_esp = this->cpu.read_gpreg32(GPRegister32::ESP);
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EAX));
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::ECX));
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDX));
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EBX));
    this->cpu.push32(old_esp);
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EBP));
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::ESI));
    this->cpu.push32(this->cpu.read_gpreg32(GPRegister32::EDI));
}

void InstructionDecoder::popa16()
{
    this->cpu.write_gpreg16((GPRegister16::DI), this->cpu.pop16());
    this->cpu.write_gpreg16((GPRegister16::SI), this->cpu.pop16());
    this->cpu.write_gpreg16((GPRegister16::BP), this->cpu.pop16());
    this->cpu.pop16();  // Discard SP
    this->cpu.write_gpreg16((GPRegister16::BX), this->cpu.pop16());
    this->cpu.write_gpreg16((GPRegister16::DX), this->cpu.pop16());
    this->cpu.write_gpreg16((GPRegister16::CX), this->cpu.pop16());
    this->cpu.write_gpreg16((GPRegister16::AX), this->cpu.pop16());
}

void InstructionDecoder::popa32()
{
    this->cpu.write_gpreg32((GPRegister32::EDI), this->cpu.pop32());
    this->cpu.write_gpreg32((GPRegister32::ESI), this->cpu.pop32());
    this->cpu.write_gpreg32((GPRegister32::EBP), this->cpu.pop32());
    this->cpu.pop32();  // Discard SP
    this->cpu.write_gpreg32((GPRegister32::EBX), this->cpu.pop32());
    this->cpu.write_gpreg32((GPRegister32::EDX), this->cpu.pop32());
    this->cpu.write_gpreg32((GPRegister32::ECX), this->cpu.pop32());
    this->cpu.write_gpreg32((GPRegister32::EAX), this->cpu.pop32());
}

void InstructionDecoder::push_imm16()
{
    this->cpu.push16(this->cpu.read_instruction16());
}

void InstructionDecoder::push_imm32()
{
    this->cpu.push32(this->cpu.read_instruction32());
}

void InstructionDecoder::imul_r16_rm16_imm16()
{
    this->write_modrm_r16(this->imul<uint16_t, int32_t>(
        this->read_modrm_rm16(), this->cpu.read_instruction16()));
}

void InstructionDecoder::imul_r32_rm32_imm32()
{
    this->write_modrm_r32(this->imul<uint32_t, int64_t>(
        this->read_modrm_rm32(), this->cpu.read_instruction32()));
}

void InstructionDecoder::push_imm8()
{
    this->cpu.push16(this->cpu.read_instruction8());
}

void InstructionDecoder::imul_r16_rm16_imm8()
{
    this->write_modrm_r16(this->imul<uint16_t, int32_t>(
        this->read_modrm_rm16(), this->cpu.read_instruction8()));
}

void InstructionDecoder::imul_r32_rm32_imm8()
{
    this->write_modrm_r32(this->imul<uint32_t, int64_t>(
        this->read_modrm_rm32(), this->cpu.read_instruction8()));
}

void InstructionDecoder::jo()
{
    this->cpu.jmpcc8(this->cpu.get_of());
}

void InstructionDecoder::jno()
{
    this->cpu.jmpcc8(!this->cpu.get_of());
}

void InstructionDecoder::jb_jnae_jc()
{
    this->cpu.jmpcc8(this->cpu.get_cf());
}

void InstructionDecoder::jnb_jae_jnc()
{
    this->cpu.jmpcc8(!this->cpu.get_cf());
}

void InstructionDecoder::jz_je()
{
    this->cpu.jmpcc8(this->cpu.get_zf());
}

void InstructionDecoder::jnz_jne()
{
    this->cpu.jmpcc8(!this->cpu.get_zf());
}

void InstructionDecoder::jbe_jna()
{
    this->cpu.jmpcc8(this->cpu.get_cf() || this->cpu.get_zf());
}

void InstructionDecoder::jnbe_ja()
{
    this->cpu.jmpcc8(!this->cpu.get_cf() && !this->cpu.get_zf());
}

void InstructionDecoder::js()
{
    this->cpu.jmpcc8(this->cpu.get_sf());
}

void InstructionDecoder::jns()
{
    this->cpu.jmpcc8(!this->cpu.get_sf());
}

void InstructionDecoder::jp_jpe()
{
    this->cpu.jmpcc8(this->cpu.get_pf());
}

void InstructionDecoder::jnp_jpo()
{
    this->cpu.jmpcc8(!this->cpu.get_pf());
}

void InstructionDecoder::jl_jnge()
{
    this->cpu.jmpcc8(!this->cpu.get_sf() != !this->cpu.get_of());
}

void InstructionDecoder::jnl_jge()
{
    this->cpu.jmpcc8(!this->cpu.get_sf() == !this->cpu.get_of());
}

void InstructionDecoder::jle_jng()
{
    this->cpu.jmpcc8(!this->cpu.get_sf() != !this->cpu.get_of() ||
                     this->cpu.get_zf());
}

void InstructionDecoder::jnle_jg()
{
    this->cpu.jmpcc8(!this->cpu.get_sf() == !this->cpu.get_of() &&
                     !this->cpu.get_zf());
}

void InstructionDecoder::do_rm8_imm8()
{
    this->load_modrm();
    uint8_t val = this->read_modrm_rm8();
    uint8_t imm = this->cpu.read_instruction8();
    switch (this->modrm->reg) {
        case 0:
            this->write_modrm_rm8(this->add(val, imm));
            break;
        case 1:
            this->write_modrm_rm8(this->do_or(val, imm));
            break;
        case 2:
            this->write_modrm_rm8(this->adc(val, imm));
            break;
        case 3:
            this->write_modrm_rm8(this->sbb(val, imm));
            break;
        case 4:
            this->write_modrm_rm8(this->do_and(val, imm));
            break;
        case 5:
            this->write_modrm_rm8(this->sub(val, imm));
            break;
        case 6:
            this->write_modrm_rm8(this->do_xor(val, imm));
            break;
        case 7:
            this->cmp(val, imm);
            break;
    }
}

void InstructionDecoder::do_rm16_imm16()
{
    this->load_modrm();
    uint16_t val = this->read_modrm_rm16();
    uint16_t imm = this->cpu.read_instruction16();
    switch (this->modrm->reg) {
        case 0:
            this->write_modrm_rm16(this->add(val, imm));
            break;
        case 1:
            this->write_modrm_rm16(this->do_or(val, imm));
            break;
        case 2:
            this->write_modrm_rm16(this->adc(val, imm));
            break;
        case 3:
            this->write_modrm_rm16(this->sbb(val, imm));
            break;
        case 4:
            this->write_modrm_rm16(this->do_and(val, imm));
            break;
        case 5:
            this->write_modrm_rm16(this->sub(val, imm));
            break;
        case 6:
            this->write_modrm_rm16(this->do_xor(val, imm));
            break;
        case 7:
            this->cmp(val, imm);
            break;
    }
}

void InstructionDecoder::do_rm32_imm32()
{
    this->load_modrm();
    uint32_t val = this->read_modrm_rm32();
    uint32_t imm = this->cpu.read_instruction32();
    switch (this->modrm->reg) {
        case 0:
            this->write_modrm_rm32(this->add(val, imm));
            break;
        case 1:
            this->write_modrm_rm32(this->do_or(val, imm));
            break;
        case 2:
            this->write_modrm_rm32(this->adc(val, imm));
            break;
        case 3:
            this->write_modrm_rm32(this->sbb(val, imm));
            break;
        case 4:
            this->write_modrm_rm32(this->do_and(val, imm));
            break;
        case 5:
            this->write_modrm_rm32(this->sub(val, imm));
            break;
        case 6:
            this->write_modrm_rm32(this->do_xor(val, imm));
            break;
        case 7:
            this->cmp(val, imm);
            break;
    }
}

void InstructionDecoder::do_rm16_imm8()
{
    this->load_modrm();
    uint16_t val = this->read_modrm_rm16();
    uint16_t imm = static_cast<uint16_t>(this->cpu.read_instruction8());
    switch (this->modrm->reg) {
        case 0:
            this->write_modrm_rm16(this->add(val, imm));
            break;
        case 1:
            this->write_modrm_rm16(this->do_or(val, imm));
            break;
        case 2:
            this->write_modrm_rm16(this->adc(val, imm));
            break;
        case 3:
            this->write_modrm_rm16(this->sbb(val, imm));
            break;
        case 4:
            this->write_modrm_rm16(this->do_and(val, imm));
            break;
        case 5:
            this->write_modrm_rm16(this->sub(val, imm));
            break;
        case 6:
            this->write_modrm_rm16(this->do_xor(val, imm));
            break;
        case 7:
            this->cmp(val, imm);
            break;
    }
}

void InstructionDecoder::do_rm32_imm8()
{
    this->load_modrm();
    uint32_t val = this->read_modrm_rm32();
    uint32_t imm = static_cast<uint32_t>(this->cpu.read_instruction8());
    switch (this->modrm->reg) {
        case 0:
            this->write_modrm_rm32(this->add(val, imm));
            break;
        case 1:
            this->write_modrm_rm32(this->do_or(val, imm));
            break;
        case 2:
            this->write_modrm_rm32(this->adc(val, imm));
            break;
        case 3:
            this->write_modrm_rm32(this->sbb(val, imm));
            break;
        case 4:
            this->write_modrm_rm32(this->do_and(val, imm));
            break;
        case 5:
            this->write_modrm_rm32(this->sub(val, imm));
            break;
        case 6:
            this->write_modrm_rm32(this->do_xor(val, imm));
            break;
        case 7:
            this->cmp(val, imm);
            break;
    }
}

void InstructionDecoder::test_rm8_r8()
{
    this->test(this->read_modrm_rm8(), this->read_modrm_r8());
}

void InstructionDecoder::test_rm16_r16()
{
    this->test(this->read_modrm_rm16(), this->read_modrm_r16());
}

void InstructionDecoder::test_rm32_r32()
{
    this->test(this->read_modrm_rm32(), this->read_modrm_r32());
}

void InstructionDecoder::xchg_rm8_r8()
{
    this->load_modrm();
    uint8_t tmp = this->read_modrm_rm8();
    this->write_modrm_rm8(this->read_modrm_r8());
    this->write_modrm_r8(tmp);
}

void InstructionDecoder::xchg_rm16_r16()
{
    this->load_modrm();
    uint16_t tmp = this->read_modrm_rm16();
    this->write_modrm_rm16(this->read_modrm_r16());
    this->write_modrm_r16(tmp);
}

void InstructionDecoder::xchg_rm32_r32()
{
    this->load_modrm();
    uint32_t tmp = this->read_modrm_rm32();
    this->write_modrm_rm32(this->read_modrm_r32());
    this->write_modrm_r32(tmp);
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

void InstructionDecoder::mov_rm16_sreg()
{
    this->load_modrm();
    this->write_modrm_rm16(this->cpu.read_sgreg(this->decode_modrm_sgreg()));
}

void InstructionDecoder::mov_rm32_sreg()
{
    this->load_modrm();
    this->write_modrm_rm32(this->cpu.read_sgreg(this->decode_modrm_sgreg()));
}

void InstructionDecoder::lea_rm16_m()
{
    // TODO: Is this even correct
    this->load_modrm();
    // Dummy call to load modrm_address
    this->read_modrm_rm16();
    // TODO: UD if not memory location
    this->write_modrm_rm16(
        this->modrm_to_address(this->modrm->mod, this->modrm->rm));
}

void InstructionDecoder::lea_rm32_m()
{
    // TODO: Is this even correct
    this->load_modrm();
    // Dummy call to load modrm_address
    this->read_modrm_rm32();
    // TODO: UD if not memory location
    this->write_modrm_rm32(
        this->modrm_to_address(this->modrm->mod, this->modrm->rm));
}

void InstructionDecoder::mov_sgreg_rm16()
{
    this->load_modrm();
    this->cpu.write_sgreg(this->decode_modrm_sgreg(), this->read_modrm_rm16());
}

void InstructionDecoder::pop_rm16()
{
    // TODO: Handle possible page fault
    this->load_modrm();
    this->write_modrm_rm16(this->cpu.pop16());
}

void InstructionDecoder::pop_rm32()
{
    this->load_modrm();
    this->write_modrm_rm32(this->cpu.pop32());
}

void InstructionDecoder::mov_al_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AL, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_cl_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::CL, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_dl_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::DL, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_bl_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::BL, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_ah_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::AH, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_ch_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::CH, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_dh_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::DH, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_bh_imm8()
{
    this->cpu.write_gpreg8(GPRegister8::BH, this->cpu.read_instruction8());
}

void InstructionDecoder::mov_ax_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::AX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_eax_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EAX, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_cx_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::CX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_ecx_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::ECX, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_dx_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::DX, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_edx_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EDX, this->cpu.read_instruction32());
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

void InstructionDecoder::mov_bp_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::BP, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_ebp_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EBP, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_si_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::SI, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_esi_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::ESI, this->cpu.read_instruction32());
}

void InstructionDecoder::mov_di_imm16()
{
    this->cpu.write_gpreg16(GPRegister16::DI, this->cpu.read_instruction16());
}

void InstructionDecoder::mov_edi_imm32()
{
    this->cpu.write_gpreg32(GPRegister32::EDI, this->cpu.read_instruction32());
}

void InstructionDecoder::retn16()
{
    this->cpu.write_ip(this->cpu.pop16());
}

void InstructionDecoder::retn32()
{
    this->cpu.write_eip(this->cpu.pop32());
}

void InstructionDecoder::in_al_imm8()
{
    // TODO: Permission checks
    this->cpu.write_gpreg8(GPRegister8::AL,
                           this->cpu.inb(this->cpu.read_instruction8()));
}

void InstructionDecoder::in_ax_imm8()
{
    this->cpu.write_gpreg16(GPRegister16::AX,
                            this->cpu.inw(this->cpu.read_instruction8()));
}

void InstructionDecoder::in_eax_imm8()
{
    this->cpu.write_gpreg32(GPRegister32::EAX,
                            this->cpu.inl(this->cpu.read_instruction8()));
}

void InstructionDecoder::out_imm8_al()
{
    // TODO: Permission checks
    this->cpu.outb(this->cpu.read_instruction8(),
                   this->cpu.read_gpreg8(GPRegister8::AL));
}

void InstructionDecoder::out_imm8_ax()
{
    this->cpu.outw(this->cpu.read_instruction8(),
                   this->cpu.read_gpreg16(GPRegister16::AX));
}

void InstructionDecoder::out_imm8_eax()
{
    this->cpu.outl(this->cpu.read_instruction8(),
                   this->cpu.read_gpreg32(GPRegister32::EAX));
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

void InstructionDecoder::jmp_rel16()
{
    int16_t off = static_cast<int16_t>(this->cpu.read_instruction16());
    this->cpu.write_ip(this->cpu.read_ip() + off);
}

void InstructionDecoder::jmp_rel32()
{
    int32_t off = static_cast<int32_t>(this->cpu.read_instruction32());
    this->cpu.write_eip(this->cpu.read_eip() + off);
}

void InstructionDecoder::jmpf_ptr16()
{
    uint16_t ip = this->cpu.read_instruction16();
    uint16_t cs = this->cpu.read_instruction16();
    this->cpu.write_sgreg(SGRegister::CS, cs);
    this->cpu.write_ip(ip);
}

void InstructionDecoder::jmpf_ptr32()
{
    uint32_t ip = this->cpu.read_instruction32();
    uint16_t cs = this->cpu.read_instruction16();
    this->cpu.write_sgreg(SGRegister::CS, cs);
    this->cpu.write_eip(ip);
}

void InstructionDecoder::hlt()
{
    this->cpu.halt();
}

void InstructionDecoder::cli()
{
    // TODO: Security checks
    this->cpu.eflags &= ~eflags_if;
}

void InstructionDecoder::cld()
{
    // TODO: Security checks
    this->cpu.eflags &= ~eflags_df;
}

void InstructionDecoder::table_ops()
{
    this->load_modrm();
    addr_t addr = this->modrm_to_address(this->modrm->mod, this->modrm->rm);
    switch (this->modrm->reg) {
        case 2:
            // lgdt
            // TODO: Security checks
            struct GDTDescriptor gdt;
            gdt.size = this->cpu.read_mem16(addr);
            gdt.offset = this->cpu.read_mem32(addr);
            if (!this->is_osize_32()) {
                // Truncated to 24-bits if osize is 16-bits
                gdt.offset &= 0xFFFFFF;
            }
            this->cpu.lgdt(gdt);
            break;
        case 3:
            // lidt
            break;
        default:
            this->log->warn(
                "[decode] Unimplemented reg value for 0xF 0x1, 0x{:X}",
                this->modrm->reg);
    }
}

void InstructionDecoder::jnz_jne16()
{
    this->cpu.jmpcc16(!this->cpu.get_zf());
}

void InstructionDecoder::jnz_jne32()
{
    this->cpu.jmpcc32(!this->cpu.get_zf());
}