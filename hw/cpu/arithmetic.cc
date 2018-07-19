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