#ifndef _OPCODES_H
#define _OPCODES_H

#include "oric_machine.h"

#include <stdio.h>

#define label(l) \
    l:

//#define NDEBUG

extern int trace_level;

#ifndef NDEBUG
#define subroutine(f) \
    void f() { \
	if (trace_level != 0) cpu_trace("entering subroutine " #f "\n");	\
f:
#define end_subroutine if (trace_level != 0) cpu_trace("exiting subroutine\n"); }
#define trace_opcode(op,ncycles) cpu_add_cycles(ncycles,2);	\
    if (trace_level > 2) printf("%05d: %s\n",__LINE__,#op)

#define trace_opcode_arg(op,arg,ncycles) cpu_add_cycles(ncycles,2);\
    if (trace_level > 2) printf("%05d: %s\t%s\n",__LINE__,#op,#arg)

#else
#define subroutine(f) \
    void f() { \
f:
#define end_subroutine }
// instruction size "averaged" to 2
#define trace_opcode(op,ncycles) cpu_add_cycles(ncycles,2)
#define trace_opcode_arg(op,arg,ncycles) cpu_add_cycles(ncycles,2)
#endif

#define unsupported_opcode(op) printf("unsupported %s\n",#op);

#define adc(param) trace_opcode_arg(adc,param,2); cpu_adc(cpu_read_mem(param))
#define adc_X(param) trace_opcode_arg(adc_X,param,2); ADC_ABS_r(param,x)
#define adc_Y(param) trace_opcode_arg(adc_Y,param,2); ADC_ABS_r(param,y)
#define adc_imm(param) trace_opcode_arg(adc_imm,param,2); cpu_adc(param)
#define sbc(param) trace_opcode_arg(sbc,param,2); cpu_sbc(cpu_read_mem(param))
#define sbc_imm(param) trace_opcode_arg(sbc_imm,param,2); cpu_sbc(param)
#define sbc_X(param) trace_opcode_arg(sbc_X,param,2); SBC_ABS_r(param,x)
#define sbc_Y(param) trace_opcode_arg(sbc_Y,param,2); SBC_ABS_r(param,y)
#define ora(param) trace_opcode_arg(ora,param,2); cpu_ora(cpu_read_mem(param))
#define and(param) trace_opcode_arg(and,param,2); cpu_and(cpu_read_mem(param))
#define and_X(param) trace_opcode_arg(and_X,param,2); cpu_and(cpu_read_mem_idx(param,r.x))
#define and_Y(param) trace_opcode_arg(and_Y,param,2); cpu_and(cpu_read_mem_idx(param,r.y))
#define and_imm(param) trace_opcode_arg(and_imm,param,2); cpu_and(param)
#define asl(param) trace_opcode_arg(asl,param,2); cpu_asl(param)
#define asl_X(param) trace_opcode_arg(asl_X,param,2); cpu_asl_x(param)
#define asl_A(param) trace_opcode_arg(asl_A,param,2); cpu_asl_a()
#define rol_A(param) trace_opcode_arg(rol_A,param,2); cpu_rol_a()
#define lsr_A(param) trace_opcode_arg(lsr_A,param,2); cpu_lsr_a()
#define ror_A(param) trace_opcode_arg(ror_A,param,2); cpu_ror_a()
#define bcc(param) trace_opcode_arg(bcc,param,2); if (!r.c) goto param
#define bcs(param) trace_opcode_arg(bcs,param,2); if (r.c) goto param
#define bvs(param) trace_opcode_arg(bvs,param,2); if (r.v) goto param
#define bvc(param) trace_opcode_arg(bvc,param,2); if (!r.v) goto param
#define beq(param) trace_opcode_arg(beq,param,2); if (r.z) goto param
#define bmi(param) trace_opcode_arg(bmi,param,2); if (r.n) goto param
#define bne(param) trace_opcode_arg(bne,param,2); if (!r.z) goto param
#define bpl(param) trace_opcode_arg(bpl,param,2); if (!r.n) goto param
#define jmp(param) {trace_opcode_arg(jmp,param,2); goto param;}
#define jsr(param) trace_opcode_arg(jsr,param,2); param()
#define jmpl(param) {trace_opcode_arg(jmpl,param,2); param(); return;}
#define jmps(param) {trace_opcode_arg(jmps,param,2); param(); return;}
#define bit(param) trace_opcode_arg(bit,param,2); cpu_bit(cpu_read_mem(param))
#define clc trace_opcode(clc,1); r.c = 0
#define cld trace_opcode(cld,1); r.d = 0
#define cli trace_opcode(cli,1); r.i = 0
#define clv trace_opcode(clv,1); r.v = 0
#define cmp(param) trace_opcode_arg(cmp,param,2); CPr_ABS(a,param)
#define cmp_ind_Y(param) trace_opcode_arg(cmp_ind_Y,param,2); cpu_cmp_ind_y(param)
#define cmp_X(param) trace_opcode_arg(cmp_X,param,2); CPr_ABS_r(a,param,x)
#define cmp_Y(param) trace_opcode_arg(cmp_Y,param,2); CPr_ABS_r(a,param,y)
#define cmp_imm(param) trace_opcode_arg(cmp_imm,param,2); CPr_IMM(a,param)
#define cpx(param) trace_opcode_arg(cpx,param,2); CPr_ABS(x,param)
#define cpx_imm(param) trace_opcode_arg(cpx_imm,param,2); CPr_IMM(x,param)
#define cpy(param) trace_opcode_arg(cpy,param,2); CPr_ABS(y,param)
#define cpy_imm(param) trace_opcode_arg(cpy_imm,param,2); CPr_IMM(y,param)
#define dec(param) trace_opcode_arg(dec,param,2); cpu_dec_memory(param)
#define dec_X(param) trace_opcode_arg(dec_X,param,2); cpu_dec_memory(param+r.x)
#define dec_Y(param) trace_opcode_arg(dec_Y,param,2); cpu_dec_memory(param+r.y)
#define inc_X(param) trace_opcode_arg(inc_X,param,2); cpu_inc_memory(param+r.x)
#define inc_Y(param) trace_opcode_arg(inc_Y,param,2); cpu_inc_memory(param+r.y)
#define dex trace_opcode(dex,1); cpu_dec(r.x)
#define dey trace_opcode(dey,1); cpu_dec(r.y)
#define eor(param) trace_opcode_arg(eor,param,2); cpu_eor(cpu_read_mem(param))
#define eor_imm(param) trace_opcode_arg(eor_imm,param,2); cpu_eor(param)
#define inc(param) trace_opcode_arg(inc,param,2); cpu_inc_memory(param)
#define inx trace_opcode(inx,1); cpu_inc(r.x)
#define iny trace_opcode(iny,1); cpu_inc(r.y)
#define jmp_ind(param) trace_opcode_arg(jmp_ind,param,2); unsupported_opcode(jmp_ind)
#define lda_X(param) trace_opcode_arg(lda_X,param,2); LDr_ABS_r(a,param,x)
#define lda_Y(param) trace_opcode_arg(lda_Y,param,2); LDr_ABS_r(a,param,y)
#define lda_imm(param) trace_opcode_arg(lda_imm,param,2); LDr_IMM(a,param)
#define ldx_imm(param) trace_opcode_arg(ldx_imm,param,2); LDr_IMM(x,param)
#define ldy_imm(param) trace_opcode_arg(ldy_imm,param,2); LDr_IMM(y,param)
#define lda(param) trace_opcode_arg(lda,param,2); LDr_ABS(a,param)
#define ldx(param) trace_opcode_arg(ldx,param,2); LDr_ABS(x,param)
#define ldy(param) trace_opcode_arg(ldy,param,2); LDr_ABS(y,param)
#define ldy_X(param) trace_opcode_arg(ldy_X,param,2); LDr_ABS_r(y,param,x)
#define ldx_Y(param) trace_opcode_arg(ldx_Y,param,2); LDr_ABS_r(x,param,y)
#define lsr(param) trace_opcode_arg(lsr,param,2); cpu_lsr(param)
#define lsr_X(param) trace_opcode_arg(lsr_X,param,2); cpu_lsr_x(param)
#define rol_X(param) trace_opcode_arg(rol_X,param,2); cpu_rol_x(param)
#define ror_X(param) trace_opcode_arg(ror_X,param,2); cpu_ror_x(param)
#define nop trace_opcode(nop,1)
#define ora_imm(param) trace_opcode_arg(ora_imm,param,2); cpu_ora(param)
#define ora_ind_X(param) trace_opcode_arg(ora_ind_X,param,2); cpu_ora_ind_x(param)
#define ora_ind_Y(param) trace_opcode_arg(ora_ind_Y,param,2); cpu_ora_ind_y(param)
#define and_ind_X(param) trace_opcode_arg(and_ind_X,param,2); cpu_and_ind_x(param)
#define and_ind_Y(param) trace_opcode_arg(and_ind_Y,param,2); cpu_and_ind_y(param)
#define pha trace_opcode(pha,1); cpu_push_byte(r.a)
#define php trace_opcode(php,1); cpu_php()
#define pla trace_opcode(pla,1); cpu_set_a(cpu_pop_byte())
#define plp trace_opcode(plp,1); cpu_plp()
#define rol(param) trace_opcode_arg(rol,param,2); cpu_rol(param)
#define ror(param) trace_opcode_arg(ror,param,2); cpu_ror(param)
#define rts trace_opcode(rts,2); return
#define sec trace_opcode(sec,1); r.c = 1
#define sed trace_opcode(sed,1); r.d = 1
#define sei trace_opcode(sei,1); r.i = 1
#define sta(param) trace_opcode_arg(sta,param,2); STr_ABS(a,param)
#define sta_X(param) trace_opcode_arg(sta_X,param,2); STr_ABS_r(a,param,x)
#define sta_Y(param) trace_opcode_arg(sta_Y,param,2); STr_ABS_r(a,param,y)
#define stx_Y(param) trace_opcode_arg(stx_Y,param,2); STr_ABS_r(x,param,y)
#define stx(param) trace_opcode_arg(stx,param,2); STr_ABS(x,param)
#define sty(param) trace_opcode_arg(sty,param,2); STr_ABS(y,param)
#define txs trace_opcode(txs,1); r.s = r.x
#define tsx trace_opcode(tsx,1); cpu_set_x(r.s)
#define tax trace_opcode(tax,1); cpu_set_x(r.a)
#define tay trace_opcode(tay,1); cpu_set_y(r.a)
#define txa trace_opcode(txa,1); cpu_set_a(r.x)
#define tya trace_opcode(tya,1); cpu_set_a(r.y)
#define lda_ind_X(param) trace_opcode_arg(lda_ind_X,param,5); cpu_lda_ind_x(param)
#define lda_ind_Y(param) trace_opcode_arg(lda_ind_Y,param,5); cpu_lda_ind_y(param)
#define sta_ind_X(param) trace_opcode_arg(sta_ind_X,param,5); cpu_sta_ind_x(param)
#define sta_ind_Y(param) trace_opcode_arg(sta_ind_Y,param,5); cpu_sta_ind_y(param)
#define sbc_ind_Y(param) trace_opcode_arg(sbc_ind_Y,param,5); cpu_sbc_ind_y(param)
#define adc_ind_Y(param) trace_opcode_arg(adc_ind_Y,param,5); cpu_adc_ind_y(param)
#define adc_ind_X(param) trace_opcode_arg(adc_ind_X,param,5); cpu_adc_ind_x(param)

// extended opcodes
#define move16(param1,param2) trace_opcode_arg(move16,param1,10); cpu_write_mem((param1) / 0x100, param2+1); cpu_write_mem((param1) & 0xff, param2)
#define ill cpu_illegal()
#define clr(param) cpu_write_mem(0,param); r.z = 1; r.n = 0

#define LDr_IMM(r1,param) cpu_set_##r1(param)
#define LDr_ABS(r1,param) cpu_set_##r1(cpu_read_mem(param))
#define LDr_ABS_r(r1,param,r2) cpu_set_##r1(cpu_read_mem_idx(param,r.r2))
#define STr_ABS(r1,param) cpu_write_mem(r.r1,param)
#define STr_ABS_r(r1,param,r2) cpu_write_mem_idx(r.r1,param,r.r2)
#define ADC_ABS_r(param,r2) cpu_adc(cpu_read_mem_idx(param,r.r2))
#define SBC_ABS_r(param,r2) cpu_sbc(cpu_read_mem_idx(param,r.r2))

#define CPr_IMM(r1,param) cpu_cmp(r.r1,param)
#define CPr_ABS(r1,param) cpu_cmp(r.r1,cpu_read_mem(param))
#define CPr_ABS_r(r1,param,r2) cpu_cmp(r.r1,cpu_read_mem_idx(param,r.r2))
#endif
