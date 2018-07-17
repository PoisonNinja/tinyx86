#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

const struct ModRM* InstructionDecoder::raw_to_modrm()
{
    return reinterpret_cast<struct ModRM*>(&this->modrm);
}

addr_t InstructionDecoder::modrm_to_address(uint8_t mod, uint8_t rm)
{
    // TODO: Switch based on real/protected mode
    if (mod == 0) {
        switch (rm) {
            case 0:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::SI)) &
                                        0xFFFF);
                break;
            case 1:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::DI)) &
                                        0xFFFF);
                break;
            case 2:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::SI)) &
                                        0xFFFF);
                break;
            case 3:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::DI)) &
                                        0xFFFF);
                break;
            case 4:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, this->cpu.read_gpreg16(GPRegister16::SI));
                break;
            case 5:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, this->cpu.read_gpreg16(GPRegister16::DI));
                break;
            case 6:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, this->cpu.read_instruction16());
                break;
            case 7:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, this->cpu.read_gpreg16(GPRegister16::BX));
                break;
        }
    } else if (mod == 1) {
        switch (rm) {
            case 0:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 1:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 2:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 3:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 4:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 5:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 6:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
            case 7:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_instruction8()) &
                                        0xFFFF);
                break;
        }
    } else if (mod == 2) {
        switch (rm) {
            case 0:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 1:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 2:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 3:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 4:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::SI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 5:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::DI) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 6:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BP) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
            case 7:
                return this->cpu.segment_to_linear(
                    SGRegister::DS, (this->cpu.read_gpreg16(GPRegister16::BX) +
                                     this->cpu.read_instruction16()) &
                                        0xFFFF);
                break;
        }
    }
    // Shut up Clang
    return 0;
}

uint8_t InstructionDecoder::read_modrm_r8()
{
    return this->cpu.read_gpreg8(
        static_cast<GPRegister8>(this->raw_to_modrm()->reg));
}

uint8_t InstructionDecoder::read_modrm_rm8()
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg8(static_cast<GPRegister8>(rm));
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem8(addr);
    }
}

uint16_t InstructionDecoder::read_modrm_r16()
{
    return this->cpu.read_gpreg16(
        static_cast<GPRegister16>(this->raw_to_modrm()->reg));
}

uint16_t InstructionDecoder::read_modrm_rm16()
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg16(static_cast<GPRegister16>(rm));
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem16(addr);
    }
}

uint32_t InstructionDecoder::read_modrm_r32()
{
    return this->cpu.read_gpreg32(
        static_cast<GPRegister32>(this->raw_to_modrm()->reg));
}

uint32_t InstructionDecoder::read_modrm_rm32()
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        return this->cpu.read_gpreg32(static_cast<GPRegister32>(rm));
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        return this->cpu.read_mem32(addr);
    }
}

void InstructionDecoder::write_modrm_r8(uint8_t value)
{
    this->cpu.write_gpreg8(static_cast<GPRegister8>(this->raw_to_modrm()->reg),
                           value);
}

void InstructionDecoder::write_modrm_rm8(uint8_t value)
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        this->cpu.write_gpreg8(static_cast<GPRegister8>(rm), value);
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        this->cpu.write_mem8(addr, value);
    }
}

void InstructionDecoder::write_modrm_r16(uint16_t value)
{
    this->cpu.write_gpreg16(
        static_cast<GPRegister16>(this->raw_to_modrm()->reg), value);
}

void InstructionDecoder::write_modrm_rm16(uint16_t value)
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        this->cpu.write_gpreg16(static_cast<GPRegister16>(rm), value);
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        this->cpu.write_mem16(addr, value);
    }
}

void InstructionDecoder::write_modrm_r32(uint32_t value)
{
    this->cpu.write_gpreg32(
        static_cast<GPRegister32>(this->raw_to_modrm()->reg), value);
}

void InstructionDecoder::write_modrm_rm32(uint32_t value)
{
    uint8_t mod = this->raw_to_modrm()->mod;
    uint8_t rm = this->raw_to_modrm()->rm;
    if (mod == 3) {
        this->cpu.write_gpreg32(static_cast<GPRegister32>(rm), value);
    } else {
        addr_t addr = this->modrm_to_address(mod, rm);
        this->cpu.write_mem32(addr, value);
    }
}
