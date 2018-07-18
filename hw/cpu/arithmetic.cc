#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

template <typename T>
T InstructionDecoder::ando(T a, T b)
{
    uint32_t result = a & b;
    this->cpu.set_eflag_operation(eflags_pf | eflags_zf | eflags_sf, 0, a, b,
                                  result, 0, sizeof(T) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::ando(uint8_t, uint8_t);
template uint16_t InstructionDecoder::ando(uint16_t, uint16_t);
template uint32_t InstructionDecoder::ando(uint32_t, uint32_t);

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
