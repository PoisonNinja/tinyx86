#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

void PrefixState::reset()
{
    segment = SGRegister::DS;
    opsize = false;
    addrsize = false;
    repne = 0;
    repe = 0;
}

InstructionDecoder::InstructionDecoder(CPU& cpu) : cpu(cpu)
{
    this->log = spdlog::get("stdout");

    this->modrm = reinterpret_cast<struct ModRM*>(&this->raw_modrm);

    this->reset();

    for (size_t i = 0; i < 256; i++) {
        this->opcodes16[i] = nullptr;
        this->opcodes32[i] = nullptr;
    }

    // Initialize instruction table
    this->opcodes16[0x00] = this->opcodes32[0x00] =
        &InstructionDecoder::add_rm8_r8;
    this->opcodes16[0x01] = &InstructionDecoder::add_rm16_r16;
    this->opcodes32[0x01] = &InstructionDecoder::add_rm32_r32;
    this->opcodes16[0x02] = this->opcodes32[0x02] =
        &InstructionDecoder::add_r8_rm8;
    this->opcodes16[0x03] = &InstructionDecoder::add_r16_rm16;
    this->opcodes32[0x03] = &InstructionDecoder::add_r32_rm32;
    this->opcodes16[0x04] = this->opcodes32[0x04] =
        &InstructionDecoder::add_al_imm8;
    this->opcodes16[0x05] = &InstructionDecoder::add_ax_imm16;
    this->opcodes32[0x05] = &InstructionDecoder::add_eax_imm32;
    this->opcodes16[0x06] = &InstructionDecoder::push_es16;
    this->opcodes32[0x06] = &InstructionDecoder::push_es32;
    this->opcodes16[0x07] = &InstructionDecoder::pop_es16;
    this->opcodes32[0x07] = &InstructionDecoder::pop_es32;
    this->opcodes16[0x08] = this->opcodes32[0x08] =
        &InstructionDecoder::or_rm8_r8;
    this->opcodes16[0x09] = &InstructionDecoder::or_rm16_r16;
    this->opcodes32[0x09] = &InstructionDecoder::or_rm32_r32;
    this->opcodes16[0x0A] = this->opcodes32[0x0A] =
        &InstructionDecoder::or_r8_rm8;
    this->opcodes16[0x0B] = &InstructionDecoder::or_r16_rm16;
    this->opcodes32[0x0B] = &InstructionDecoder::or_r32_rm32;
    this->opcodes16[0x0C] = this->opcodes32[0x0C] =
        &InstructionDecoder::or_al_imm8;
    this->opcodes16[0x0D] = &InstructionDecoder::or_ax_imm16;
    this->opcodes32[0x0D] = &InstructionDecoder::or_eax_imm32;
    this->opcodes16[0x0E] = &InstructionDecoder::push_cs16;
    this->opcodes32[0x0E] = &InstructionDecoder::push_cs32;
    this->opcodes16[0x10] = this->opcodes32[0x10] =
        &InstructionDecoder::adc_rm8_r8;
    this->opcodes16[0x11] = &InstructionDecoder::adc_rm16_r16;
    this->opcodes32[0x11] = &InstructionDecoder::adc_rm32_r32;
    this->opcodes16[0x12] = this->opcodes32[0x12] =
        &InstructionDecoder::adc_r8_rm8;
    this->opcodes16[0x13] = &InstructionDecoder::adc_r16_rm16;
    this->opcodes32[0x13] = &InstructionDecoder::adc_r32_rm32;
    this->opcodes16[0x14] = this->opcodes32[0x14] =
        &InstructionDecoder::adc_al_imm8;
    this->opcodes16[0x15] = &InstructionDecoder::adc_ax_imm16;
    this->opcodes32[0x15] = &InstructionDecoder::adc_eax_imm32;
    this->opcodes16[0x16] = &InstructionDecoder::push_ss16;
    this->opcodes32[0x16] = &InstructionDecoder::push_ss32;
    this->opcodes16[0x17] = &InstructionDecoder::pop_ss16;
    this->opcodes32[0x17] = &InstructionDecoder::pop_ss32;
    this->opcodes16[0x18] = this->opcodes32[0x18] =
        &InstructionDecoder::sbb_rm8_r8;
    this->opcodes16[0x19] = &InstructionDecoder::sbb_rm16_r16;
    this->opcodes32[0x19] = &InstructionDecoder::sbb_rm32_r32;
    this->opcodes16[0x1A] = this->opcodes32[0x1A] =
        &InstructionDecoder::sbb_r8_rm8;
    this->opcodes16[0x1B] = &InstructionDecoder::sbb_r16_rm16;
    this->opcodes32[0x1B] = &InstructionDecoder::sbb_r32_rm32;
    this->opcodes16[0x1C] = this->opcodes32[0x1C] =
        &InstructionDecoder::sbb_al_imm8;
    this->opcodes16[0x1D] = &InstructionDecoder::sbb_ax_imm16;
    this->opcodes32[0x1D] = &InstructionDecoder::sbb_eax_imm32;
    this->opcodes16[0x1E] = &InstructionDecoder::push_ds16;
    this->opcodes32[0x1E] = &InstructionDecoder::push_ds32;
    this->opcodes16[0x1F] = &InstructionDecoder::pop_ds16;
    this->opcodes32[0x1F] = &InstructionDecoder::pop_ds32;
    this->opcodes16[0x20] = this->opcodes32[0x20] =
        &InstructionDecoder::and_rm8_r8;
    this->opcodes16[0x43] = &InstructionDecoder::inc_bx;
    this->opcodes32[0x43] = &InstructionDecoder::inc_ebx;
    this->opcodes16[0x47] = &InstructionDecoder::inc_di;
    this->opcodes32[0x47] = &InstructionDecoder::inc_edi;
    this->opcodes16[0x52] = &InstructionDecoder::push_dx;
    this->opcodes32[0x52] = &InstructionDecoder::push_edx;
    this->opcodes16[0x53] = &InstructionDecoder::push_bx;
    this->opcodes32[0x53] = &InstructionDecoder::push_ebx;
    this->opcodes16[0x57] = &InstructionDecoder::push_di;
    this->opcodes32[0x57] = &InstructionDecoder::push_edi;
    this->opcodes16[0x5A] = &InstructionDecoder::pop_dx;
    this->opcodes32[0x5A] = &InstructionDecoder::pop_edx;
    this->opcodes16[0x5B] = &InstructionDecoder::pop_bx;
    this->opcodes32[0x5B] = &InstructionDecoder::pop_ebx;
    this->opcodes16[0x5F] = &InstructionDecoder::pop_di;
    this->opcodes32[0x5F] = &InstructionDecoder::pop_edi;
    this->opcodes16[0x75] = this->opcodes32[0x75] =
        &InstructionDecoder::jmpnz_ne;
    this->opcodes16[0x86] = this->opcodes32[0x86] =
        &InstructionDecoder::xchg_r8_rm8;
    this->opcodes16[0x88] = this->opcodes32[0x88] =
        &InstructionDecoder::mov_rm8_r8;
    this->opcodes16[0x89] = &InstructionDecoder::mov_rm16_r16;
    this->opcodes32[0x89] = &InstructionDecoder::mov_rm32_r32;
    this->opcodes16[0x8A] = this->opcodes32[0x8A] =
        &InstructionDecoder::mov_r8_rm8;
    this->opcodes16[0x8B] = &InstructionDecoder::mov_r16_rm16;
    this->opcodes32[0x8B] = &InstructionDecoder::mov_r32_rm32;
    this->opcodes16[0xB8] = &InstructionDecoder::mov_ax_imm16;
    this->opcodes32[0xB8] = &InstructionDecoder::mov_eax_imm32;
    this->opcodes16[0xBB] = &InstructionDecoder::mov_bx_imm16;
    this->opcodes32[0xBB] = &InstructionDecoder::mov_ebx_imm32;
    this->opcodes16[0xBC] = &InstructionDecoder::mov_sp_imm16;
    this->opcodes32[0xBC] = &InstructionDecoder::mov_esp_imm32;
    this->opcodes16[0xC3] = &InstructionDecoder::retn16;
    this->opcodes32[0xC3] = &InstructionDecoder::retn32;
    this->opcodes16[0xE8] = &InstructionDecoder::call_rel16;
    this->opcodes32[0xE8] = &InstructionDecoder::call_rel32;
    this->opcodes16[0xEA] = &InstructionDecoder::jmpf_ptr16;
    this->opcodes32[0xEA] = &InstructionDecoder::jmpf_ptr32;
    this->opcodes16[0xF4] = this->opcodes32[0xF4] = &InstructionDecoder::hlt;
}

InstructionDecoder::~InstructionDecoder()
{
}

void InstructionDecoder::tick()
{
    this->prefixes.reset();
    uint8_t opcode = this->cpu.read_instruction8();
    bool isPrefix = true;
    while (isPrefix) {
        switch (opcode) {
            case 0x2E:
                this->log->trace("[decode] Prefix overriding to CS");
                this->prefixes.segment = SGRegister::CS;
                break;
            case 0x66:
                this->log->trace("[decode] Prefix overriding operand size");
                this->prefixes.opsize = true;
                break;
            case 0x67:
                this->log->trace("[decode] Prefix overriding address size");
                this->prefixes.addrsize = true;
                break;
            default:
                isPrefix = false;
                break;
        }
    }
    this->log->trace("[decode] Opcode: 0x{:X}", opcode);
    // TODO: Check for 32 bit. We assume 16 bit by default
    InstructionPointer* opcodes =
        (this->is_osize_32()) ? this->opcodes32 : this->opcodes16;
    if (!opcodes[opcode]) {
        this->log->error("[decode] Unknown opcode 0x{:X}, halting...", opcode);
        this->cpu.stop();
    } else {
        InstructionPointer instr = opcodes[opcode];
        (this->*instr)();
    }
}

void InstructionDecoder::reset()
{
    this->raw_modrm = 0;
}

bool InstructionDecoder::is_osize_32()
{
    return this->prefixes.opsize != this->cpu.is_32();
}

bool InstructionDecoder::is_asize_32()
{
    return this->prefixes.addrsize != this->cpu.is_32();
}