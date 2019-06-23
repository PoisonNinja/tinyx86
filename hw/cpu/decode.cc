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
        this->fopcodes16[i] = nullptr;
        this->fopcodes32[i] = nullptr;
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
    this->opcodes16[0x21] = &InstructionDecoder::and_rm16_r16;
    this->opcodes32[0x21] = &InstructionDecoder::and_rm32_r32;
    this->opcodes16[0x22] = this->opcodes32[0x22] =
        &InstructionDecoder::and_r8_rm8;
    this->opcodes16[0x23] = &InstructionDecoder::and_r16_rm16;
    this->opcodes32[0x23] = &InstructionDecoder::and_r32_rm32;
    this->opcodes16[0x24] = this->opcodes32[0x24] =
        &InstructionDecoder::and_al_imm8;
    this->opcodes16[0x25] = &InstructionDecoder::and_ax_imm16;
    this->opcodes32[0x25] = &InstructionDecoder::and_eax_imm32;
    this->opcodes16[0x27] = this->opcodes32[0x27] = &InstructionDecoder::daa_al;
    this->opcodes16[0x28] = this->opcodes32[0x28] =
        &InstructionDecoder::sub_rm8_r8;
    this->opcodes16[0x29] = &InstructionDecoder::sub_rm16_r16;
    this->opcodes32[0x29] = &InstructionDecoder::sub_rm32_r32;
    this->opcodes16[0x2A] = this->opcodes32[0x2A] =
        &InstructionDecoder::sub_r8_rm8;
    this->opcodes16[0x2B] = &InstructionDecoder::sub_r16_rm16;
    this->opcodes32[0x2B] = &InstructionDecoder::sub_r32_rm32;
    this->opcodes16[0x2C] = this->opcodes32[0x2C] =
        &InstructionDecoder::sub_al_imm8;
    this->opcodes16[0x2D] = &InstructionDecoder::sub_ax_imm16;
    this->opcodes32[0x2D] = &InstructionDecoder::sub_eax_imm32;
    this->opcodes16[0x2F] = this->opcodes32[0x2F] = &InstructionDecoder::das_al;
    this->opcodes16[0x30] = this->opcodes32[0x30] =
        &InstructionDecoder::xor_rm8_r8;
    this->opcodes16[0x31] = &InstructionDecoder::xor_rm16_r16;
    this->opcodes32[0x31] = &InstructionDecoder::xor_rm32_r32;
    this->opcodes16[0x32] = this->opcodes32[0x32] =
        &InstructionDecoder::xor_r8_rm8;
    this->opcodes16[0x33] = &InstructionDecoder::xor_r16_rm16;
    this->opcodes32[0x33] = &InstructionDecoder::xor_r32_rm32;
    this->opcodes16[0x34] = this->opcodes32[0x34] =
        &InstructionDecoder::xor_al_imm8;
    this->opcodes16[0x35] = &InstructionDecoder::xor_ax_imm16;
    this->opcodes32[0x35] = &InstructionDecoder::xor_eax_imm32;
    this->opcodes16[0x37] = this->opcodes32[0x37] =
        &InstructionDecoder::aaa_al_ah;
    this->opcodes16[0x38] = this->opcodes32[0x38] =
        &InstructionDecoder::cmp_rm8_r8;
    this->opcodes16[0x39] = &InstructionDecoder::cmp_rm16_r16;
    this->opcodes32[0x39] = &InstructionDecoder::cmp_rm32_r32;
    this->opcodes16[0x3A] = this->opcodes32[0x3A] =
        &InstructionDecoder::cmp_r8_rm8;
    this->opcodes16[0x3B] = &InstructionDecoder::cmp_r16_rm16;
    this->opcodes32[0x3B] = &InstructionDecoder::cmp_r32_rm32;
    this->opcodes16[0x3C] = this->opcodes32[0x3C] =
        &InstructionDecoder::cmp_al_imm8;
    this->opcodes16[0x3D] = &InstructionDecoder::cmp_ax_imm16;
    this->opcodes32[0x3D] = &InstructionDecoder::cmp_eax_imm32;
    this->opcodes16[0x3F] = this->opcodes32[0x3F] =
        &InstructionDecoder::aas_al_ah;
    this->opcodes16[0x40] = &InstructionDecoder::inc_ax;
    this->opcodes32[0x40] = &InstructionDecoder::inc_eax;
    this->opcodes16[0x41] = &InstructionDecoder::inc_cx;
    this->opcodes32[0x41] = &InstructionDecoder::inc_ecx;
    this->opcodes16[0x42] = &InstructionDecoder::inc_dx;
    this->opcodes32[0x42] = &InstructionDecoder::inc_edx;
    this->opcodes16[0x43] = &InstructionDecoder::inc_bx;
    this->opcodes32[0x43] = &InstructionDecoder::inc_ebx;
    this->opcodes16[0x44] = &InstructionDecoder::inc_sp;
    this->opcodes32[0x44] = &InstructionDecoder::inc_esp;
    this->opcodes16[0x45] = &InstructionDecoder::inc_bp;
    this->opcodes32[0x45] = &InstructionDecoder::inc_ebp;
    this->opcodes16[0x46] = &InstructionDecoder::inc_si;
    this->opcodes32[0x46] = &InstructionDecoder::inc_esi;
    this->opcodes16[0x47] = &InstructionDecoder::inc_di;
    this->opcodes32[0x47] = &InstructionDecoder::inc_edi;
    this->opcodes16[0x48] = &InstructionDecoder::dec_ax;
    this->opcodes32[0x48] = &InstructionDecoder::dec_eax;
    this->opcodes16[0x49] = &InstructionDecoder::dec_cx;
    this->opcodes32[0x49] = &InstructionDecoder::dec_ecx;
    this->opcodes16[0x4A] = &InstructionDecoder::dec_dx;
    this->opcodes32[0x4A] = &InstructionDecoder::dec_edx;
    this->opcodes16[0x4B] = &InstructionDecoder::dec_bx;
    this->opcodes32[0x4B] = &InstructionDecoder::dec_ebx;
    this->opcodes16[0x4C] = &InstructionDecoder::dec_sp;
    this->opcodes32[0x4C] = &InstructionDecoder::dec_esp;
    this->opcodes16[0x4D] = &InstructionDecoder::dec_bp;
    this->opcodes32[0x4D] = &InstructionDecoder::dec_ebp;
    this->opcodes16[0x4E] = &InstructionDecoder::dec_si;
    this->opcodes32[0x4E] = &InstructionDecoder::dec_esi;
    this->opcodes16[0x4F] = &InstructionDecoder::dec_di;
    this->opcodes32[0x4F] = &InstructionDecoder::dec_edi;
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
    this->opcodes16[0x83] = &InstructionDecoder::do_rm16_imm8;
    this->opcodes32[0x83] = &InstructionDecoder::do_rm32_imm8;
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

    this->fopcodes16[0x85] = &InstructionDecoder::jnz_jne16;
    this->fopcodes32[0x85] = &InstructionDecoder::jnz_jne16;
}

InstructionDecoder::~InstructionDecoder()
{
}

void InstructionDecoder::tick()
{
    this->prefixes.reset();
    uint8_t opcode = 0;  // Will be fetched below
    bool isPrefix = true;
    while (isPrefix) {
        switch ((opcode = this->cpu.read_instruction8())) {
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
    InstructionPointer* opcodes;
    if (opcode == 0xF) {
        // Two byte opcode
        opcode = this->cpu.read_instruction8();
        this->log->trace("[decode] Opcode: 0xF 0x{:X}", opcode);
        opcodes = (this->is_osize_32()) ? this->fopcodes32 : this->fopcodes16;
    } else {
        this->log->trace("[decode] Opcode: 0x{:X}", opcode);
        opcodes = (this->is_osize_32()) ? this->opcodes32 : this->opcodes16;
    }
    // TODO: Check for 32 bit. We assume 16 bit by default
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