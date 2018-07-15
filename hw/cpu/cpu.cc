#include <hw/board.h>
#include <hw/cpu/cpu.h>

EFLAGS::EFLAGS()
{
}

EFLAGS::~EFLAGS()
{
}

void EFLAGS::reset()
{
    this->eflags = 0;
    this->eflags_dirty = 0;
    this->last_op1 = 0;
    this->last_op2 = 0;
    this->last_result = 0;
    this->last_add_result = 0;
    this->last_size = 0;
}

uint32_t EFLAGS::get_eflags()
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

bool EFLAGS::get_cf()
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

bool EFLAGS::get_pf()
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

bool EFLAGS::get_af()
{
    if (eflags_dirty & eflags_af) {
        eflags |= ((last_op1 ^ last_op2 ^ last_add_result) & eflags_af) ?
                      eflags_af :
                      0;
        eflags_dirty &= ~eflags_af;
    }
    return eflags & eflags_af;
}

bool EFLAGS::get_zf()
{
    if (eflags_dirty & eflags_zf) {
        eflags |=
            ((~last_result & last_result - 1) >> last_size & 1) ? eflags_zf : 0;
        eflags_dirty &= ~eflags_zf;
    }
    return eflags & eflags_zf;
}

bool EFLAGS::get_sf()
{
    if (eflags_dirty & eflags_sf) {
        eflags |= ((last_result >> last_size) & 1) ? eflags_sf : 0;
        eflags_dirty &= ~eflags_sf;
    }
    return eflags & eflags_sf;
}

bool EFLAGS::get_of()
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
    this->state = CPUState::RUNNING;
    this->decoder.tick();
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
    this->eflags.reset();

    // Set to reset vector
    this->ip.regs_32 = 0;
}

uint8_t CPU::read_gpreg8(GPRegister reg)
{
    return gpregs[static_cast<int>(reg)].regs_8;
}

uint16_t CPU::read_gpreg16(GPRegister reg)
{
    return gpregs[static_cast<int>(reg)].regs_16;
}

uint32_t CPU::read_gpreg32(GPRegister reg)
{
    return gpregs[static_cast<int>(reg)].regs_32;
}

void CPU::write_gpreg8(GPRegister reg, uint8_t value)
{
    gpregs[static_cast<int>(reg)].regs_8 = value;
}

void CPU::write_gpreg16(GPRegister reg, uint16_t value)
{
    gpregs[static_cast<int>(reg)].regs_16 = value;
}

void CPU::write_gpreg32(GPRegister reg, uint32_t value)
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
    union Register& sp = this->gpregs[static_cast<int>(GPRegister::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, (sp.regs_16 - 2));
    write_mem16(addr, value);
    sp.regs_16 -= 2;
}

void CPU::push32(uint32_t value)
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_16 - 4);
    write_mem32(addr, value);
    sp.regs_16 -= 4;
}

uint16_t CPU::pop16()
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_16);
    return sp.regs_16 += 2, read_mem16(addr);
}

uint32_t CPU::pop32()
{
    union Register& sp = this->gpregs[static_cast<int>(GPRegister::SP)];
    addr_t addr = segment_to_linear(SGRegister::SS, sp.regs_32);
    return sp.regs_16 += 4, read_mem32(addr);
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
