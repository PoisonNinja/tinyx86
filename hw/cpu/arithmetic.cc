#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

template <typename T>
T InstructionDecoder::adc(T a, T b)
{
    uint32_t result = a + b + ((this->cpu.get_cf()) ? 1 : 0);
    this->cpu.set_eflag_operation(eflags_all, 0, a, b, result, result,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::adc(uint8_t, uint8_t);
template uint16_t InstructionDecoder::adc(uint16_t, uint16_t);
template uint32_t InstructionDecoder::adc(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::add(T a, T b)
{
    uint32_t result = a + b;
    this->cpu.set_eflag_operation(eflags_all, 0, a, b, result, result,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::add(uint8_t, uint8_t);
template uint16_t InstructionDecoder::add(uint16_t, uint16_t);
template uint32_t InstructionDecoder::add(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::do_and(T a, T b)
{
    uint32_t result = a & b;
    this->cpu.set_eflag_operation(eflags_pf | eflags_zf | eflags_sf, 0, a, b,
                                  result, 0, sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::do_and(uint8_t, uint8_t);
template uint16_t InstructionDecoder::do_and(uint16_t, uint16_t);
template uint32_t InstructionDecoder::do_and(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::bsf(T a, T b)
{
    uint32_t flags = eflags_all & ~eflags_zf;
    uint32_t initial = 0;
    uint32_t result = 0;
    if (b == 0) {
        initial |= eflags_zf;
        result = a;
    } else {
        // uint*_t are implicitly promoted to double
        result = std::log2(b & ~b);
    }
    // Everything else besides ZF is undefined so result and size only matter
    this->cpu.set_eflag_operation(flags, initial, a, b, result, 0,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint16_t InstructionDecoder::bsf(uint16_t, uint16_t);
template uint32_t InstructionDecoder::bsf(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::bsr(T a, T b)
{
    uint32_t flags = eflags_all & ~eflags_zf;
    uint32_t initial = 0;
    uint32_t result = 0;
    if (b == 0) {
        initial |= eflags_zf;
        result = a;
    } else {
        // uint*_t are implicitly promoted to double
        result = std::log2(b);
    }
    // Everything else besides ZF is undefined so result and size only matter
    this->cpu.set_eflag_operation(flags, initial, a, b, result, 0,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint16_t InstructionDecoder::bsr(uint16_t, uint16_t);
template uint32_t InstructionDecoder::bsr(uint32_t, uint32_t);

template <typename T>
void InstructionDecoder::cmp(T a, T b)
{
    /*
     * cmp is implemented in terms of sub. cmp just discards the results
     */
    this->sub(a, b);
}
template void InstructionDecoder::cmp(uint8_t, uint8_t);
template void InstructionDecoder::cmp(uint16_t, uint16_t);
template void InstructionDecoder::cmp(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::dec(T v)
{
    uint32_t initial = this->cpu.get_cf() ? eflags_cf : 0;
    uint32_t result = static_cast<uint32_t>(v) - 1;
    this->cpu.set_eflag_operation((eflags_all & ~eflags_cf), initial, result, 1,
                                  result, v, sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::dec(uint8_t);
template uint16_t InstructionDecoder::dec(uint16_t);
template uint32_t InstructionDecoder::dec(uint32_t);

template <typename T>
void InstructionDecoder::div(T a)
{
    // a == divisor
    if (a == 0) {
        // TODO: Throw division error
        this->log->warn("Division by 0. We should probably throw an exception");
    }
    if (sizeof(T) == 1) {
        uint16_t dividend = this->cpu.read_gpreg16(GPRegister16::AX);
        uint16_t quotient = dividend / a;
        uint8_t mod = dividend % a;
        if (quotient > 0xFF) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg8(GPRegister8::AL, quotient);
        this->cpu.write_gpreg8(GPRegister8::AH, mod);
    } else if (sizeof(T) == 2) {
        uint32_t dividend = (this->cpu.read_gpreg16(GPRegister16::DX) << 16) |
                            (this->cpu.read_gpreg16(GPRegister16::AX));
        uint32_t quotient = dividend / a;
        uint16_t mod = dividend % a;
        if (quotient > 0xFFFF) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg16(GPRegister16::AX, quotient);
        this->cpu.write_gpreg16(GPRegister16::DX, mod);
    } else if (sizeof(T) == 4) {
        uint64_t dividend =
            (static_cast<uint64_t>(this->cpu.read_gpreg32(GPRegister32::EDX))
             << 32) |
            (this->cpu.read_gpreg32(GPRegister32::EAX));
        uint64_t quotient = dividend / a;
        uint32_t mod = dividend % a;
        if (quotient > 0xFFFFFFFF) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg32(GPRegister32::EAX, quotient);
        this->cpu.write_gpreg32(GPRegister32::EDX, mod);
    }
    // All flags are undefined after div so no need to set last op
}
template void InstructionDecoder::div(uint8_t);
template void InstructionDecoder::div(uint16_t);
template void InstructionDecoder::div(uint32_t);

template <typename T>
void InstructionDecoder::idiv(T a)
{
    // a == divisor
    if (a == 0) {
        // TODO: Throw division error
        this->log->warn("Division by 0. We should probably throw an exception");
    }
    if (sizeof(T) == 1) {
        int16_t dividend = this->cpu.read_gpreg16(GPRegister16::AX);
        int16_t quotient = dividend / a;
        int8_t mod = dividend % a;
        if (std::abs(quotient) > 0x7F) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg8(GPRegister8::AL, quotient);
        this->cpu.write_gpreg8(GPRegister8::AH, mod);
    } else if (sizeof(T) == 2) {
        int32_t dividend =
            (static_cast<int32_t>(this->cpu.read_gpreg16(GPRegister16::DX))
             << 16) |
            (this->cpu.read_gpreg16(GPRegister16::AX));
        int32_t quotient = dividend / a;
        int16_t mod = dividend % a;
        if (std::abs(quotient) > 0x7FFF) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg16(GPRegister16::AX, quotient);
        this->cpu.write_gpreg16(GPRegister16::DX, mod);
    } else if (sizeof(T) == 4) {
        int64_t dividend =
            (static_cast<int64_t>(this->cpu.read_gpreg32(GPRegister32::EDX))
             << 32) |
            (this->cpu.read_gpreg32(GPRegister32::EAX));
        int64_t quotient = dividend / a;
        int32_t mod = dividend % a;
        if (std::abs(quotient) > 0x7FFFFFFF) {
            this->log->warn(
                "Division by 0. We should probably throw an exception");
        }
        this->cpu.write_gpreg32(GPRegister32::EAX, quotient);
        this->cpu.write_gpreg32(GPRegister32::EDX, mod);
    }
    // All flags are undefined after idiv so no need to set last op
}
template void InstructionDecoder::idiv(uint8_t);
template void InstructionDecoder::idiv(uint16_t);
template void InstructionDecoder::idiv(uint32_t);

template <typename T>
T InstructionDecoder::inc(T v)
{
    uint32_t initial = this->cpu.get_cf() ? eflags_cf : 0;
    uint32_t result = static_cast<uint32_t>(v) + 1;
    this->cpu.set_eflag_operation((eflags_all & ~eflags_cf), initial, v, 1,
                                  result, result, sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::inc(uint8_t);
template uint16_t InstructionDecoder::inc(uint16_t);
template uint32_t InstructionDecoder::inc(uint32_t);

template <typename T>
void InstructionDecoder::mul(T a)
{
    // a == factor
    uint32_t initial = 0;
    if (sizeof(T) == 1) {
        uint8_t factor = this->cpu.read_gpreg8(GPRegister8::AL);
        uint16_t product = factor * a;
        this->cpu.write_gpreg16(GPRegister16::AX, product);
        if (this->cpu.read_gpreg8(GPRegister8::AH)) {
            initial |= eflags_of | eflags_cf;
        }
    } else if (sizeof(T) == 2) {
        uint16_t factor = this->cpu.read_gpreg16(GPRegister16::AX);
        uint32_t product = factor * a;
        this->cpu.write_gpreg16(GPRegister16::AX, product & 0xFFFF);
        this->cpu.write_gpreg16(GPRegister16::DX, product >> 16);
        if (this->cpu.read_gpreg16(GPRegister16::DX)) {
            initial |= eflags_of | eflags_cf;
        }
    } else if (sizeof(T) == 4) {
        uint32_t factor = this->cpu.read_gpreg32(GPRegister32::EAX);
        uint64_t product = factor * a;
        this->cpu.write_gpreg32(GPRegister32::EAX, product & 0xFFFFFFFF);
        this->cpu.write_gpreg32(GPRegister32::EDX, product >> 32);
        if (this->cpu.read_gpreg32(GPRegister32::EDX)) {
            initial |= eflags_of | eflags_cf;
        }
    }
    this->cpu.set_eflag_operation(0, initial, 0, 0, 0, 0, 0);
}
template void InstructionDecoder::mul(uint8_t);
template void InstructionDecoder::mul(uint16_t);
template void InstructionDecoder::mul(uint32_t);

template <typename T>
T InstructionDecoder::do_or(T a, T b)
{
    uint32_t result = a | b;
    this->cpu.set_eflag_operation(eflags_pf | eflags_zf | eflags_sf, 0, a, b,
                                  result, result, sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::do_or(uint8_t, uint8_t);
template uint16_t InstructionDecoder::do_or(uint16_t, uint16_t);
template uint32_t InstructionDecoder::do_or(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::sbb(T a, T b)
{
    uint32_t result = a - b - ((this->cpu.get_cf()) ? 1 : 0);
    this->cpu.set_eflag_operation(eflags_all, 0, result, b, result, a,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::sbb(uint8_t, uint8_t);
template uint16_t InstructionDecoder::sbb(uint16_t, uint16_t);
template uint32_t InstructionDecoder::sbb(uint32_t, uint32_t);

template <typename T>
T InstructionDecoder::sub(T a, T b)
{
    uint32_t result = a - b;
    this->cpu.set_eflag_operation(eflags_all, 0, result, b, result, a,
                                  sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::sub(uint8_t, uint8_t);
template uint16_t InstructionDecoder::sub(uint16_t, uint16_t);
template uint32_t InstructionDecoder::sub(uint32_t, uint32_t);