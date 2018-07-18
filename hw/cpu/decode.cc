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
    this->opcodes16[0x52] = &InstructionDecoder::push_dx;
    this->opcodes32[0x52] = &InstructionDecoder::push_edx;
    this->opcodes16[0x53] = &InstructionDecoder::push_bx;
    this->opcodes32[0x53] = &InstructionDecoder::push_ebx;
    this->opcodes16[0xB8] = &InstructionDecoder::mov_ax_imm16;
    this->opcodes32[0xB8] = &InstructionDecoder::mov_eax_imm32;
    this->opcodes16[0xBC] = &InstructionDecoder::mov_sp_imm16;
    this->opcodes32[0xBC] = &InstructionDecoder::mov_esp_imm32;
    this->opcodes16[0xE8] = &InstructionDecoder::call_rel16;
    this->opcodes32[0xE8] = &InstructionDecoder::call_rel32;
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
                this->log->trace("Prefix overriding to CS");
                this->prefixes.segment = SGRegister::CS;
                break;
            case 0x66:
                this->log->trace("Prefix overriding operand size");
                this->prefixes.opsize = true;
                break;
            default:
                isPrefix = false;
                break;
        }
    }
    this->log->trace("Opcode: 0x{:X}", opcode);
    // TODO: Check for 32 bit. We assume 16 bit by default
    InstructionPointer* opcodes =
        (this->is_osize_32()) ? this->opcodes32 : this->opcodes16;
    if (!opcodes[opcode]) {
        this->log->error("Unknown opcode 0x{:X}, halting...", opcode);
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