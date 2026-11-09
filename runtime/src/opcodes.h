#ifndef _OPCODES_H
#define _OPCODES_H

#include "oric_machine.h"

#include <stdio.h>


extern int trace_level;

#ifdef NDEBUG
#define _bra(param) goto label_##param
#define param_size(param) 0
#else
#define _bra(param) do { if (param < 0xC000) { r.pc=param; } goto label_##param; } while(0)
#define param_size(param) ((param < 0x100) ? 2 : 3)
#endif

#define trace_opcode(op,ncycles,isize) cpu_add_cycles(ncycles,isize)
#define trace_opcode_arg(op,arg,ncycles,isize) cpu_add_cycles(ncycles,isize)

#define unsupported_opcode(op) printf("unsupported %s\n",#op);

#define adc(param) do { trace_opcode_arg(adc,param,2,param_size(param)); cpu_adc(cpu_read_mem(param)); } while(0)
#define adc_X(param) do { trace_opcode_arg(adc_X,param,2,param_size(param)); ADC_ABS_r(param,x); } while(0)
#define adc_Y(param) do { trace_opcode_arg(adc_Y,param,2,3); ADC_ABS_r(param,y); } while(0)
#define adc_imm(param) do { trace_opcode_arg(adc_imm,param,2,2); cpu_adc(param); } while(0)
#define sbc(param) do { trace_opcode_arg(sbc,param,2,param_size(param)); cpu_sbc(cpu_read_mem(param)); } while(0)
#define sbc_imm(param) do { trace_opcode_arg(sbc_imm,param,2,2); cpu_sbc(param); } while(0)
#define sbc_X(param) do { trace_opcode_arg(sbc_X,param,2,param_size(param)); SBC_ABS_r(param,x); } while(0)
#define sbc_Y(param) do { trace_opcode_arg(sbc_Y,param,2,3); SBC_ABS_r(param,y); } while(0)
#define ora(param) do { trace_opcode_arg(ora,param,2,param_size(param)); cpu_ora(cpu_read_mem(param)); } while(0)
#define and(param) do { trace_opcode_arg(and,param,2,param_size(param)); cpu_and(cpu_read_mem(param)); } while(0)
#define and_X(param) do { trace_opcode_arg(and_X,param,2,param_size(param)); cpu_and(cpu_read_mem_idx(param,r.x)); } while(0)
#define and_Y(param) do { trace_opcode_arg(and_Y,param,2,3); cpu_and(cpu_read_mem_idx(param,r.y)); } while(0)
#define and_imm(param) do { trace_opcode_arg(and_imm,param,2,2); cpu_and(param); } while(0)
#define asl(param) do { trace_opcode_arg(asl,param,2,param_size(param)); cpu_asl(param); } while(0)
#define asl_X(param) do { trace_opcode_arg(asl_X,param,2,param_size(param)); cpu_asl_x(param); } while(0)
#define asl_A do { trace_opcode_arg(asl_A,param,2,1); cpu_asl_a(); } while(0)
#define rol_A do { trace_opcode_arg(rol_A,param,2,1); cpu_rol_a(); } while(0)
#define lsr_A do { trace_opcode_arg(lsr_A,param,2,1); cpu_lsr_a(); } while(0)
#define ror_A do { trace_opcode_arg(ror_A,param,2,1); cpu_ror_a(); } while(0)

#define bcc(param) do { trace_opcode_arg(bcc,param,2,2); if (!r.c) _bra(param); } while(0)
#define bcs(param) do { trace_opcode_arg(bcs,param,2,2); if (r.c) _bra(param); } while(0)
#define bvs(param) do { trace_opcode_arg(bvs,param,2,2); if (r.v) _bra(param); } while(0)
#define bvc(param) do { trace_opcode_arg(bvc,param,2,2); if (!r.v) _bra(param); } while(0)
#define beq(param) do { trace_opcode_arg(beq,param,2,2); if (r.z) _bra(param); } while(0)
#define bmi(param) do { trace_opcode_arg(bmi,param,2,2); if (r.n) _bra(param); } while(0)
#define bne(param) do { trace_opcode_arg(bne,param,2,2); if (!r.z) _bra(param); } while(0)
#define bpl(param) do { trace_opcode_arg(bpl,param,2,2); if (!r.n) _bra(param); } while(0)
#define jmp(param) do { trace_opcode_arg(jmp,param,2,3); _bra(param); } while(0)
#ifdef NDEBUG
#define _jsr(param) do {int sstk=r.s;r.s-=2;\
   call_handler(param,__LINE__,0);\
   r.s+=2; if (r.s!=sstk) {r.s-=2;return;   }\
} while(0)
#else
#define _jsr(param) do {int sstk=r.s;r.s-=2;\
   if (param < 0xC000) {r.pc=param;}\
   call_handler(param,__LINE__,0);\
   r.s+=2; if (r.s!=sstk) {r.s-=2; printf("stack diff %x vs %x: popping return addr %x\n",r.s,sstk,r.pc); return;}\
} while(0)
#endif

#define jsr(param) do { trace_opcode_arg(jsr,param,2,3); _jsr(param); } while(0)

#define bit(param) do { trace_opcode_arg(bit,param,2,param_size(param)); cpu_bit(cpu_read_mem(param)); } while(0)
#define clc do { trace_opcode(clc,1,1); r.c = 0; } while(0)
#define cld do { trace_opcode(cld,1,1); r.d = 0; } while(0)
#define cli do { trace_opcode(cli,1,1); r.i = 0; } while(0)
#define clv do { trace_opcode(clv,1,1); r.v = 0; } while(0)
#define cmp(param) do { trace_opcode_arg(cmp,param,2,param_size(param)); CPr_ABS(a,param); } while(0)
#define cmp_ind_Y(param) do { trace_opcode_arg(cmp_ind_Y,param,2,2); cpu_cmp_ind_y(param); } while(0)
#define cmp_X(param) do { trace_opcode_arg(cmp_X,param,2,param_size(param)); CPr_ABS_r(a,param,x); } while(0)
#define cmp_Y(param) do { trace_opcode_arg(cmp_Y,param,2,3); CPr_ABS_r(a,param,y); } while(0)
#define cmp_imm(param) do { trace_opcode_arg(cmp_imm,param,2,2); CPr_IMM(a,param); } while(0)
#define cpx(param) do { trace_opcode_arg(cpx,param,2,param_size(param)); CPr_ABS(x,param); } while(0)
#define cpx_imm(param) do { trace_opcode_arg(cpx_imm,param,2,2); CPr_IMM(x,param); } while(0)
#define cpy(param) do { trace_opcode_arg(cpy,param,2,param_size(param)); CPr_ABS(y,param); } while(0)
#define cpy_imm(param) do { trace_opcode_arg(cpy_imm,param,2,2); CPr_IMM(y,param); } while(0)
#define dec(param) do { trace_opcode_arg(dec,param,2,param_size(param)); cpu_dec_memory(param); } while(0)
#define dec_X(param) do { trace_opcode_arg(dec_X,param,2,param_size(param)); cpu_dec_memory(param+r.x); } while(0)
#define dec_Y(param) do { trace_opcode_arg(dec_Y,param,2,3); cpu_dec_memory(param+r.y); } while(0)
#define inc_X(param) do { trace_opcode_arg(inc_X,param,2,param_size(param)); cpu_inc_memory(param+r.x); } while(0)
#define inc_Y(param) do { trace_opcode_arg(inc_Y,param,2,3); cpu_inc_memory(param+r.y); } while(0)
#define dex do { trace_opcode(dex,1,1); cpu_dec(r.x); } while(0)
#define dey do { trace_opcode(dey,1,1); cpu_dec(r.y); } while(0)
#define eor(param) do { trace_opcode_arg(eor,param,2,param_size(param)); cpu_eor(cpu_read_mem(param)); } while(0)
#define eor_imm(param) do { trace_opcode_arg(eor_imm,param,2,2); cpu_eor(param); } while(0)
#define inc(param) do { trace_opcode_arg(inc,param,2,param_size(param)); cpu_inc_memory(param); } while(0)
#define inx do { trace_opcode(inx,1,1); cpu_inc(r.x); } while(0)
#define iny do { trace_opcode(iny,1,1); cpu_inc(r.y); } while(0)
#define jmp_ind(param) do { trace_opcode_arg(jmp_ind,param,2,3); unsupported_opcode(jmp_ind); } while(0)
#define jsr_ind(param) jsr(cpu_read_mem(param)); } while(0)
#define lda_X(param) do { trace_opcode_arg(lda_X,param,2,param_size(param)); LDr_ABS_r(a,param,x); } while(0)
#define lda_Y(param) do { trace_opcode_arg(lda_Y,param,2,3); LDr_ABS_r(a,param,y); } while(0)
#define lda_imm(param) do { trace_opcode_arg(lda_imm,param,2,2); LDr_IMM(a,param); } while(0)
#define ldx_imm(param) do { trace_opcode_arg(ldx_imm,param,2,2); LDr_IMM(x,param); } while(0)
#define ldy_imm(param) do { trace_opcode_arg(ldy_imm,param,2,2); LDr_IMM(y,param); } while(0)
#define lda(param) do { trace_opcode_arg(lda,param,2,param_size(param)); LDr_ABS(a,param); } while(0)
#define ldx(param) do { trace_opcode_arg(ldx,param,2,param_size(param)); LDr_ABS(x,param); } while(0)
#define ldy(param) do { trace_opcode_arg(ldy,param,2,param_size(param)); LDr_ABS(y,param); } while(0)
#define ldy_X(param) do { trace_opcode_arg(ldy_X,param,2,param_size(param)); LDr_ABS_r(y,param,x); } while(0)
#define ldx_Y(param) do { trace_opcode_arg(ldx_Y,param,2,param_size(param)); LDr_ABS_r(x,param,y); } while(0)
#define lsr(param) do { trace_opcode_arg(lsr,param,2,param_size(param)); cpu_lsr(param); } while(0)
#define lsr_X(param) do { trace_opcode_arg(lsr_X,param,2,param_size(param)); cpu_lsr_x(param); } while(0)
#define rol_X(param) do { trace_opcode_arg(rol_X,param,2,param_size(param)); cpu_rol_x(param); } while(0)
#define ror_X(param) do { trace_opcode_arg(ror_X,param,2,param_size(param)); cpu_ror_x(param); } while(0)
#define nop do { trace_opcode(nop,1,1); } while(0)
#define ora_imm(param) do { trace_opcode_arg(ora_imm,param,2,2); cpu_ora(param); } while(0)
#define ora_ind_X(param) do { trace_opcode_arg(ora_ind_X,param,2,2); cpu_ora_ind_x(param); } while(0)
#define ora_ind_Y(param) do { trace_opcode_arg(ora_ind_Y,param,2,2); cpu_ora_ind_y(param); } while(0)
#define eor_X(param) do { trace_opcode_arg(eor_X,param,2,param_size(param)); EOR_ABS_r(param,x); } while(0)
#define ora_X(param) do { trace_opcode_arg(ora_X,param,2,param_size(param)); ORA_ABS_r(param,x); } while(0)
#define ora_Y(param) do { trace_opcode_arg(ora_Y,param,2,3); ORA_ABS_r(param,y); } while(0)
#define and_ind_X(param) do { trace_opcode_arg(and_ind_X,param,2,2); cpu_and_ind_x(param); } while(0)
#define and_ind_Y(param) do { trace_opcode_arg(and_ind_Y,param,2,2); cpu_and_ind_y(param); } while(0)
#define pha do { trace_opcode(pha,1,1); cpu_push_byte(r.a); } while(0)
#define php do { trace_opcode(php,1,1); cpu_php(); } while(0)
#define pla do { trace_opcode(pla,1,1); cpu_set_a(cpu_pop_byte()); } while(0)
#define plp do { trace_opcode(plp,1,1); cpu_plp(); } while(0)
#define rol(param) do { trace_opcode_arg(rol,param,2,param_size(param)); cpu_rol(param); } while(0)
#define ror(param) do { trace_opcode_arg(ror,param,2,param_size(param)); cpu_ror(param); } while(0)
#define rts do { trace_opcode(rts,2,1); return; } while(0)
#define sec do { trace_opcode(sec,1,1); r.c = 1; } while(0)
#define sed do { trace_opcode(sed,1,1); r.d = 1; } while(0)
#define sei do { trace_opcode(sei,1,1); r.i = 1; } while(0)
#define sta(param) do { trace_opcode_arg(sta,param,2,param_size(param)); STr_ABS(a,param); } while(0)
#define sta_X(param) do { trace_opcode_arg(sta_X,param,2,param_size(param)); STr_ABS_r(a,param,x); } while(0)
#define sta_Y(param) do { trace_opcode_arg(sta_Y,param,2,3); STr_ABS_r(a,param,y); } while(0)
#define stx_Y(param) do { trace_opcode_arg(stx_Y,param,2,3); STr_ABS_r(x,param,y); } while(0)
#define stx(param) do { trace_opcode_arg(stx,param,2,param_size(param)); STr_ABS(x,param); } while(0)
#define sty(param) do { trace_opcode_arg(sty,param,2,param_size(param)); STr_ABS(y,param); } while(0)
#define txs do { trace_opcode(txs,1,1); r.s = r.x; } while(0)
#define tsx do { trace_opcode(tsx,1,1); cpu_set_x(r.s); } while(0)
#define tax do { trace_opcode(tax,1,1); cpu_set_x(r.a); } while(0)
#define tay do { trace_opcode(tay,1,1); cpu_set_y(r.a); } while(0)
#define txa do { trace_opcode(txa,1,1); cpu_set_a(r.x); } while(0)
#define tya do { trace_opcode(tya,1,1); cpu_set_a(r.y); } while(0)
#define lda_ind_X(param) do { trace_opcode_arg(lda_ind_X,param,5,2); cpu_lda_ind_x(param); } while(0)
#define lda_ind_Y(param) do { trace_opcode_arg(lda_ind_Y,param,5,2); cpu_lda_ind_y(param); } while(0)
#define sta_ind_X(param) do { trace_opcode_arg(sta_ind_X,param,5,2); cpu_sta_ind_x(param); } while(0)
#define sta_ind_Y(param) do { trace_opcode_arg(sta_ind_Y,param,5,2); cpu_sta_ind_y(param); } while(0)
#define sbc_ind_Y(param) do { trace_opcode_arg(sbc_ind_Y,param,5,2); cpu_sbc_ind_y(param); } while(0)
#define adc_ind_Y(param) do { trace_opcode_arg(adc_ind_Y,param,5,2); cpu_adc_ind_y(param); } while(0)
#define adc_ind_X(param) do { trace_opcode_arg(adc_ind_X,param,5,2); cpu_adc_ind_x(param); } while(0)

// extended opcodes
#define move16(param1,param2) do { trace_opcode_arg(move16,param1,10,5); cpu_write_mem((param1) / 0x100, param2+1); cpu_write_mem((param1) & 0xff, param2); } while(0)
#define illegal(param) do { trace_opcode_arg(illegal,param,1,1); unsupported_opcode("ILLEGAL");cpu_illegal(); } while(0)
#define clr(param) do { cpu_write_mem(0,param); r.z = 1; r.n = 0; } while(0)

#define LDr_IMM(r1,param) cpu_set_##r1(param)
#define LDr_ABS(r1,param) cpu_set_##r1(cpu_read_mem(param))
#define LDr_ABS_r(r1,param,r2) cpu_set_##r1(cpu_read_mem_idx(param,r.r2))
#define EOR_ABS_r(param,r2) cpu_eor(cpu_read_mem_idx(param,r.r2))
#define ORA_ABS_r(param,r2) cpu_ora(cpu_read_mem_idx(param,r.r2))
#define STr_ABS(r1,param) cpu_write_mem(r.r1,param)
#define STr_ABS_r(r1,param,r2) cpu_write_mem_idx(r.r1,param,r.r2)
#define ADC_ABS_r(param,r2) cpu_adc(cpu_read_mem_idx(param,r.r2))
#define SBC_ABS_r(param,r2) cpu_sbc(cpu_read_mem_idx(param,r.r2))
#define CPr_IMM(r1,param) cpu_cmp(r.r1,param)
#define CPr_ABS(r1,param) cpu_cmp(r.r1,cpu_read_mem(param))
#define CPr_ABS_r(r1,param,r2) cpu_cmp(r.r1,cpu_read_mem_idx(param,r.r2))
#endif
