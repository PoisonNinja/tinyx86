#include <hw/cpu/cpu.h>

EFLAGS::EFLAGS()
{
}

EFLAGS::~EFLAGS()
{
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

CPU::CPU(Board& b) : board(b)
{
    this->reset();
}

CPU::~CPU()
{
}

void CPU::tick()
{
}

void CPU::reset()
{
    this->ip.regs_32 = 0;
}