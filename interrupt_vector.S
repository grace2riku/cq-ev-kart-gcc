/*
 *
 * CPU例外/割込みハンドラテーブルの定義
 * 各ハンドラに16byteずつ命令を配置可能
 *
 */
#include "V850asm.inc"

	.section .vectors, "ax"
	.align 4
	
	.globl	interrupt
	.globl	_default_int_handler

_reset_handler:	/* 00000000H */
	.globl _start
	.long	0xFFFFFFFF
	jr	_start
	nop
	nop
	nop
	nop
_nmi_handler:	/* 00000010H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intwdt2_handler:	/* 00000020H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int30h_handler:	/* 00000030H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_trap0n_handler:	/* 00000040H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_trap1n_handler: /* 00000050H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_ilgop_dbg0_handler: /* 00000060H */
	.long	0xFFFFFFFF
	nop
	nop
	nop
	nop
	nop
	nop
security_id: /* 00000070H */
	.long	0xFFFFFFFF
	.long	0xFFFFFFFF
	.short	0xFFFF

option_bytes: /* 0000007AH, 0000007BH */
	.byte	0x00
	.byte	0x81
	.byte	0
	.byte	0
	.byte	0
	.byte	0

_intlvil_handler: /* 00000080H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intlvih_handler: /* 00000090H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp0_handler: /* 000000A0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp1_handler: /* 000000B0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp2_handler: /* 000000C0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp3_handler: /* 000000D0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp4_handler: /* 000000E0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp5_handler: /* 000000F0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp6_handler: /* 00000100H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp7_handler: /* 00000110H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab0ov_handler: /* 00000120H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab0cc0_handler: /* 00000130H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab0cc1_handler: /* 00000140H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab0cc2_handler: /* 00000150H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab0cc3_handler: /* 00000160H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa0ov_handler: /* 00000170H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa0cc0_handler: /* 00000180H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa0cc1_handler: /* 00000190H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa1ov_handler: /* 000001A0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa1cc0_handler: /* 000001B0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa1cc1_handler: /* 000001C0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa2ov_handler: /* 000001D0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa2cc0_handler: /* 000001E0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa2cc1_handler: /* 000001F0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa3ov_handler: /* 00000200H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa3cc0_handler: /* 00000210H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa3cc1_handler: /* 00000220H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa4ov_handler: /* 00000230H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa4cc0_handler: /* 00000240H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttaa4cc1_handler: /* 00000250H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttm0eq0_handler: /* 00000260H */
	jr	_inttm0eq0_label
	nop
	nop
	nop
	nop
	nop
	nop
_intcb0r_handler: /* 00000270H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intcb0t_handler: /* 00000280H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intcb1r_handler: /* 00000290H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intcb1t_handler: /* 000002A0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intud0s_handler: /* 000002B0H */
	.long	0xFFFFFFFF
	nop
	nop
	nop
	nop
	nop
	nop
_intud0r_handler: /* 000002C0H */
	.long	0xFFFFFFFF
	nop
	nop
	nop
	nop
	nop
	nop
_intud0t_handler: /* 000002D0H */
	.long	0xFFFFFFFF
	nop
	nop
	nop
	nop
	nop
	nop
_intud1s_handler: /* 000002E0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intud1r_handler: /* 000002F0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intud1t_handler: /* 00000300H */
	jr	_intud1t_label
	nop
	nop
	nop
	nop
	nop
	nop
_intiic0_ud4s_handler: /* 00000310H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intad_handler: /* 00000320H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc0err_handler: /* 00000330H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc0wup_handler: /* 00000340H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc0rec_handler: /* 00000350H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc0trx_handler: /* 00000360H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intdma0_handler: /* 00000370H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intdma1_handler: /* 00000380H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intdma2_handler: /* 00000390H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intdma3_handler: /* 000003A0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intkr_handler: /* 000003B0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intwti_handler: /* 000003C0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intwt_handler: /* 000003D0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_int3e0_handler: /* 000003E0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intfl_handler: /* 000003F0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp8_handler: /* 00000400H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp9_handler: /* 00000410H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intp10_handler: /* 00000420H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab1ov_handler: /* 00000430H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab1cc0_handler: /* 00000440H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_inttab1cc1_handler: /* 00000450H */
	jr	_inttab1cc1_label
	nop
	nop
	nop
	nop
	nop
	nop
_inttab1cc2_handler: /* 00000460H */
	jr	_inttab1cc2_label
	nop
	nop
	nop
	nop
	nop
	nop
_inttab1cc3_handler: /* 00000470H */
	jr	_inttab1cc3_label
	nop
	nop
	nop
	nop
	nop
	nop
_intud2s_handler: /* 00000480H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intud2r_handler: /* 00000490H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intud2t_handler: /* 000004A0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc1err_handler: /* 000004B0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc1wup_handler: /* 000004C0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc1rec_handler: /* 000004D0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop
_intc1trx_handler: /* 000004E0H */
	jr	_goto_default_int_handler
	nop
	nop
	nop
	nop
	nop
	nop


	.section .text, "ax"
	.align 4

	.globl	__interrupt_tm0eq0_handler
_inttm0eq0_label:
	addi	-80,	sp, sp
	st.w	r10,	52[sp]
	st.w	r11,	48[sp]
	st.w	r12,	44[sp]
	Lea		__interrupt_tm0eq0_handler, r10
	jr		interrupt

	.globl	__interrupt_hall_sensor_u
_inttab1cc1_label:
	addi	-80,	sp, sp
	st.w	r10,	52[sp]
	st.w	r11,	48[sp]
	st.w	r12,	44[sp]
	Lea		__interrupt_hall_sensor_u, r10
	jr		interrupt
	
	.globl	__interrupt_hall_sensor_v
_inttab1cc2_label:
	addi	-80,	sp, sp
	st.w	r10,	52[sp]
	st.w	r11,	48[sp]
	st.w	r12,	44[sp]
	Lea		__interrupt_hall_sensor_v, r10
	jr		interrupt

	.globl	__interrupt_hall_sensor_w
_inttab1cc3_label:
	addi	-80,	sp, sp
	st.w	r10,	52[sp]
	st.w	r11,	48[sp]
	st.w	r12,	44[sp]
	Lea		__interrupt_hall_sensor_w, r10
	jr		interrupt

	.globl	__interrupt_send_log
_intud1t_label:
	addi	-80,	sp, sp
	st.w	r10,	52[sp]
	st.w	r11,	48[sp]
	st.w	r12,	44[sp]
	Lea		__interrupt_send_log, r10
	jr		interrupt

_goto_default_int_handler:
	addi 	-80, sp, sp
	st.w 	r10, 0[sp]
	Lea 	_default_int_handler, r10
	jr 		interrupt

	.end















