#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

template <typename T>
T InstructionDecoder::inc(T v)
{
    uint32_t initial = this->cpu.get_cf() ? eflags_cf : 0;
    uint32_t result = static_cast<uint32_t>(v) + 1;
    this->cpu.set_eflag_operation((eflags_all & ~eflags_cf), initial, v, 1,
                                  result, result, sizeof(v) * 8 - 1);
    return result;
}
template uint8_t InstructionDecoder::inc(uint8_t);
template uint16_t InstructionDecoder::inc(uint16_t);
template uint32_t InstructionDecoder::inc(uint32_t);
