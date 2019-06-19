#include <hw/board.h>
#include <hw/cpu/cpu.h>

CPU::CPU(Board& b) : board(b), decoder(*this)
{
    this->log = spdlog::get("stdout");
    this->state = CPUState::STOPPED;
    this->reset();
}

CPU::~CPU()
{
}

void CPU::tick()
{
    if (this->state == CPUState::HALTED) {
        // TODO: Check for interrupts
        this->log->info("[cpu] CPU halted, actually stopping");
        this->state = CPUState::STOPPED;
    } else {
        this->state = CPUState::RUNNING;
        this->decoder.tick();
    }
}

void CPU::reset()
{
    // Reset all registers to 0
    for (size_t i = 0; i < num_gpregs; i++) {
        this->gpregs[i].regs_32 = 0;
    }
    for (size_t i = 0; i < num_sgregs; i++) {
        this->sgregs[i].selector = 0;
        this->sgregs[i].base = 0;
        this->sgregs[i].limit = 0;
    }
    this->ip.regs_32 = 0;

    // Reset EFLAGS
    this->eflags = 0;
    this->eflags_dirty = 0;
    this->last_op1 = 0;
    this->last_op2 = 0;
    this->last_result = 0;
    this->last_add_result = 0;
    this->last_size = 0;

    // Set to reset vector
    this->sgregs[static_cast<int>(SGRegister::CS)].base = 0xFFFF0000;
    this->sgregs[static_cast<int>(SGRegister::CS)].limit = 0xFFFF;
    this->sgregs[static_cast<int>(SGRegister::CS)].selector = 0xF000;
    this->ip.regs_16 = 0xFFF0;
}

uint8_t CPU::read_gpreg8(GPRegister8 reg)
{
    if (reg <= GPRegister8::BL) {
        return gpregs[static_cast<int>(reg)].regs_8l;
    } else {
        return gpregs[static_cast<int>(reg) - static_cast<int>(GPRegister8::AH)]
            .regs_8h;
    }
}

uint16_t CPU::read_gpreg16(GPRegister16 reg)
{
    return gpregs[static_cast<int>(reg)].regs_16;
}

uint32_t CPU::read_gpreg32(GPRegister32 reg)
{
    return gpregs[static_cast<int>(reg)].regs_32;
}

void CPU::write_gpreg8(GPRegister8 reg, uint8_t value)
{
    if (reg <= GPRegister8::BL) {
        gpregs[static_cast<int>(reg)].regs_8l = value;
    } else {
        gpregs[static_cast<int>(reg) - static_cast<int>(GPRegister8::AH)]
            .regs_8h = value;
    }
}

void CPU::write_gpreg16(GPRegister16 reg, uint16_t value)
{
    gpregs[static_cast<int>(reg)].regs_16 = value;
}

void CPU::write_gpreg32(GPRegister32 reg, uint32_t value)
{
    gpregs[static_cast<int>(reg)].regs_32 = value;
}

uint16_t CPU::read_sgreg(SGRegister reg)
{
    return sgregs[static_cast<int>(reg)].selector;
}

void CPU::write_sgreg(SGRegister reg, uint16_t selector)
{
    // TODO: Protected mode
    sgregs[static_cast<int>(reg)].selector = selector;
    sgregs[static_cast<int>(reg)].base = selector << 4;
}

uint16_t CPU::read_ip()
{
    return this->ip.regs_16;
}

uint32_t CPU::read_eip()
{
    return this->ip.regs_32;
}

void CPU::write_ip(uint16_t value)
{
    this->ip.regs_16 = value;
}

void CPU::write_eip(uint32_t value)
{
    this->ip.regs_32 = value;
}

uint8_t CPU::read_mem8(addr_t addr)
{
    return board.read8(addr);
}

uint16_t CPU::read_mem16(addr_t addr)
{
    return board.read16(addr);
}

uint32_t CPU::read_mem32(addr_t addr)
{
    return board.read32(addr);
}

void CPU::write_mem8(addr_t addr, uint8_t value)
{
    board.write8(addr, value);
}

void CPU::write_mem16(addr_t addr, uint16_t value)
{
    board.write16(addr, value);
}

void CPU::write_mem32(addr_t addr, uint32_t value)
{
    board.write32(addr, value);
}

uint8_t CPU::read_instruction8()
{
    addr_t ip = segment_to_linear(SGRegister::CS, this->ip.regs_16);
    return this->ip.regs_16++, read_mem8(ip);
}

uint16_t CPU::read_instruction16()
{
    addr_t ip = segment_to_linear(SGRegister::CS, this->ip.regs_16);
    return this->ip.regs_16 += 2, read_mem16(ip);
}

uint32_t CPU::read_instruction32()
{
    addr_t ip = segment_to_linear(SGRegister::CS, this->ip.regs_32);
    return this->ip.regs_32 += 4, read_mem32(ip);
}

void CPU::push16(uint16_t value)
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister16::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, (sp.regs_16 - 2));
    write_mem16(addr, value);
    sp.regs_16 -= 2;
}

void CPU::push32(uint32_t value)
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister32::ESP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_16 - 4);
    write_mem32(addr, value);
    sp.regs_16 -= 4;
}

uint16_t CPU::pop16()
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister16::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_16);
    return sp.regs_16 += 2, read_mem16(addr);
}

uint32_t CPU::pop32()
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister32::ESP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_32);
    return sp.regs_16 += 4, read_mem32(addr);
}

bool CPU::is_32()
{
    // TODO: Eventually figure out of we are actually 32-bit based on GDT
    return false;
}

uint32_t CPU::get_eflags()
{
    get_cf();
    get_pf();
    get_af();
    get_zf();
    get_sf();
    get_of();
    return eflags;
}

/*
 * The following flag calculations are largely borrowed from v86 written by
 * Fabian Hemmer. The code is licensed under the Simplified BSD license.
 * It has been reproduced here:
 *
 * Copyright (c) 2012-2014, Fabian Hemmer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 */

bool CPU::get_cf()
{
    if (eflags_dirty & eflags_cf) {
        eflags |=
            ((last_op1 ^ ((last_op1 ^ last_op2) & (last_op2 ^ last_result))) >>
                 last_size &
             1) ?
                eflags_cf :
                0;
        eflags_dirty &= ~eflags_cf;
    }
    return eflags & eflags_cf;
}

bool CPU::get_pf()
{
    if (eflags_dirty & eflags_pf) {
        eflags |= (0x9669 << 2 >> ((last_result ^ last_result >> 4) & 0xF) &
                   eflags_pf) ?
                      eflags_pf :
                      0;
        eflags_dirty &= ~eflags_pf;
    }
    return eflags & eflags_pf;
}

bool CPU::get_af()
{
    if (eflags_dirty & eflags_af) {
        eflags |= ((last_op1 ^ last_op2 ^ last_add_result) & eflags_af) ?
                      eflags_af :
                      0;
        eflags_dirty &= ~eflags_af;
    }
    return eflags & eflags_af;
}

bool CPU::get_zf()
{
    if (eflags_dirty & eflags_zf) {
        eflags |=
            ((~last_result & last_result - 1) >> last_size & 1) ? eflags_zf : 0;
        eflags_dirty &= ~eflags_zf;
    }
    return eflags & eflags_zf;
}

bool CPU::get_sf()
{
    if (eflags_dirty & eflags_sf) {
        eflags |= ((last_result >> last_size) & 1) ? eflags_sf : 0;
        eflags_dirty &= ~eflags_sf;
    }
    return eflags & eflags_sf;
}

bool CPU::get_of()
{
    if (eflags_dirty & eflags_of) {
        eflags |=
            (((last_op1 ^ last_add_result) & (last_op2 ^ last_add_result)) >>
                 last_size &
             1) ?
                eflags_of :
                0;
    }
    return eflags & eflags_of;
}

void CPU::set_eflag_operation(uint32_t eflags_dirty, uint32_t eflags_initial,
                              uint32_t last_op1, uint32_t last_op2,
                              uint32_t last_result, uint32_t last_add_result,
                              uint8_t last_size)
{
    this->eflags &= this->eflags & 0xFFFFF72A;  // Clear all status flags
    this->eflags |=
        eflags_initial & 0x8D5;  // Set status flags based on initial
    this->eflags_dirty = eflags_dirty;
    this->last_op1 = last_op1;
    this->last_op2 = last_op2;
    this->last_result = last_result;
    this->last_add_result = last_add_result;
    this->last_size = last_size;
}

addr_t CPU::segment_to_linear(SGRegister reg, addr_t offset)
{
    return sgregs[static_cast<int>(reg)].base + offset;
}

void CPU::halt()
{
    this->state = CPUState::HALTED;
}

void CPU::stop()
{
    this->state = CPUState::STOPPED;
}

CPUState CPU::get_state()
{
    return this->state;
}
