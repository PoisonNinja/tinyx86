#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void InstructionDecoder::load_modrm()
{
    this->raw_modrm = this->cpu.read_instruction8();
}

SGRegister InstructionDecoder::decode_modrm_sgreg()
{
    switch (this->modrm->reg) {
        case 0:
            return SGRegister::ES;
        case 1:
            return SGRegister::CS;
        case 2:
            return SGRegister::SS;
        case 3:
            return SGRegister::DS;
        case 4:
            return SGRegister::FS;
        case 5:
            return SGRegister::GS;
        default:
            this->log->critical("Invalid sgregister");
            return SGRegister::CS;
    }
}

addr_t InstructionDecoder::modrm_to_address(uint8_t mod, uint8_t rm)
{
    // TODO: Switch based on real/protected mode
    if (!this->is_asize_32()) {
        if (mod == 0) {
            switch (rm) {
                case 0:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::SI)) &
                            0xFFFF);
                    break;
                case 1:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::DI)) &
                            0xFFFF);
                    break;
                case 2:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::SI)) &
                            0xFFFF);
                    break;
                case 3:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::DI)) &
                            0xFFFF);
                    break;
                case 4:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        this->cpu.read_gpreg16(GPRegister16::SI));
                    break;
                case 5:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        this->cpu.read_gpreg16(GPRegister16::DI));
                    break;
                case 6:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS, this->cpu.read_instruction16());
                    break;
                case 7:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        this->cpu.read_gpreg16(GPRegister16::BX));
                    break;
            }
        } else if (mod == 1) {
            switch (rm) {
                case 0:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 1:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 2:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 3:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 4:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 5:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 6:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
                case 7:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_instruction8()) &
                            0xFFFF);
                    break;
            }
        } else if (mod == 2) {
            switch (rm) {
                case 0:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 1:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 2:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 3:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 4:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::SI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 5:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::DI) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 6:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BP) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
                case 7:
                    return this->cpu.segment_to_linear(
                        SGRegister::DS,
                        (this->cpu.read_gpreg16(GPRegister16::BX) +
                         this->cpu.read_instruction16()) &
                            0xFFFF);
                    break;
            }
        }
    } else {
        this->log->warn("Trying to decode a 32-bit address, should probably "
                        "implement this");
    }
    // Shut up Clang
    return 0;
}

uint8_t InstructionDecoder::read_modrm_r8()
{
    return this->cpu.read_gpreg8(static_cast<GPRegister8>(this->modrm->reg));
}

uint8_t InstructionDecoder::read_modrm_rm8()
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg8(static_cast<GPRegister8>(rm));
    } else {
        this->decoded_addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem8(this->decoded_addr);
    }
}

uint16_t InstructionDecoder::read_modrm_r16()
{
    return this->cpu.read_gpreg16(static_cast<GPRegister16>(this->modrm->reg));
}

uint16_t InstructionDecoder::read_modrm_rm16()
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg16(static_cast<GPRegister16>(rm));
    } else {
        this->decoded_addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem16(this->decoded_addr);
    }
}

uint32_t InstructionDecoder::read_modrm_r32()
{
    return this->cpu.read_gpreg32(static_cast<GPRegister32>(this->modrm->reg));
}

uint32_t InstructionDecoder::read_modrm_rm32()
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg32(static_cast<GPRegister32>(rm));
    } else {
        this->decoded_addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem32(this->decoded_addr);
    }
}

void InstructionDecoder::write_modrm_r8(uint8_t value)
{
    this->cpu.write_gpreg8(static_cast<GPRegister8>(this->modrm->reg), value);
}

void InstructionDecoder::write_modrm_rm8(uint8_t value)
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        this->cpu.write_gpreg8(static_cast<GPRegister8>(rm), value);
    } else {
        this->cpu.write_mem8(this->decoded_addr, value);
    }
}

void InstructionDecoder::write_modrm_r16(uint16_t value)
{
    this->cpu.write_gpreg16(static_cast<GPRegister16>(this->modrm->reg), value);
}

void InstructionDecoder::write_modrm_rm16(uint16_t value)
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        this->cpu.write_gpreg16(static_cast<GPRegister16>(rm), value);
    } else {
        this->cpu.write_mem16(this->decoded_addr, value);
    }
}

void InstructionDecoder::write_modrm_r32(uint32_t value)
{
    this->cpu.write_gpreg32(static_cast<GPRegister32>(this->modrm->reg), value);
}

void InstructionDecoder::write_modrm_rm32(uint32_t value)
{
    uint8_t mod = this->modrm->mod;
    uint8_t rm = this->modrm->rm;
    if (mod == 3) {
        this->cpu.write_gpreg32(static_cast<GPRegister32>(rm), value);
    } else {
        this->cpu.write_mem32(this->decoded_addr, value);
    }
}
