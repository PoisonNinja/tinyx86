#include <hw/cpu/cpu.h>
#include <hw/cpu/decode.h>

InstructionDecoder::InstructionDecoder(CPU& cpu) : cpu(cpu)
{
    this->log = spdlog::get("stdout");

    this->reset();

    for (size_t i = 0; i < 256; i++) {
        this->opcodes[i] = nullptr;
    }

    // Initialize instruction table
    this->opcodes[0x00] = &InstructionDecoder::add_rm8_r8;
}

InstructionDecoder::~InstructionDecoder()
{
}

void InstructionDecoder::tick()
{
    uint8_t opcode = this->cpu.read_instruction8();
    bool isPrefix = true;
    while (isPrefix) {
        switch (opcode) {
            case 0x2E:
                this->log->trace("Prefix overriding to CS");
                this->prefixes.segment = SGRegister::CS;
                break;
            default:
                isPrefix = false;
                break;
        }
    }
    this->log->trace("Opcode: 0x{:X}", opcode);
    if (!this->opcodes[opcode]) {
        this->log->error("Unknown opcode 0x{:X}, halting...", opcode);
        this->cpu.stop();
    } else {
        InstructionPointer instr = this->opcodes[opcode];
        (this->*instr)();
    }
}

void InstructionDecoder::reset()
{
    this->modrm = 0;
}
