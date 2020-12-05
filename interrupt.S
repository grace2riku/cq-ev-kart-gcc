/*
 * ����ߥϥ�ɥ�ƤӽФ����쥸�������򡦸ƽФ���쥸������������
 * 
 */

#include "v850asm.inc"

	.globl interrupt
interrupt:
	/* 
	 * �Ĥ�Υ쥸��������, EP�ǤΥ����������ڤ��ؤ���
	 * r10-r12�ϸƤӽФ���������Ѥ�
	 */
	 st.w	ep,		12[sp]
	 mov	sp,		ep
	 sst.w	r1,		72[ep]
	 sst.w	r6,		68[ep]
	 sst.w	r7,		64[ep]
	 sst.w	r8,		60[ep]
	 sst.w	r9,		56[ep]
	 sst.w	r13,	40[ep]
	 sst.w	r14,	36[ep]
	 sst.w	r15,	32[ep]
	 sst.w	r16,	28[ep]
	 sst.w	r17,	24[ep]
	 sst.w	r18,	20[ep]
	 sst.w	r19,	16[ep]
	 sst.w	r31,	8[ep]
	 
	 /* ����襢�ɥ쥹������ */
	 Lea	end_int_handler, lp
	 
	 /* �ϥ�ɥ鵯ư */
	 jmp	r10

end_int_handler:
	mov		sp , ep	/* EP���ڤ��ؤ� */
	/* 
	 * �쥸��������
	 */
	 sld.w	72[ep],	r1
	 sld.w	68[ep],	r6
	 sld.w	64[ep],	r7
	 sld.w	60[ep],	r8
	 sld.w	56[ep],	r9
	 sld.w	52[ep],	r10
	 sld.w	48[ep],	r11
	 sld.w	44[ep],	r12
	 sld.w	40[ep],	r13
	 sld.w	36[ep],	r14
	 sld.w	32[ep],	r15
	 sld.w	28[ep],	r16
	 sld.w	24[ep],	r17
	 sld.w	20[ep],	r18
	 sld.w	16[ep],	r19
	 sld.w	8[ep],	r31
	 addi	80, ep,	sp
	 sld.w	12[ep],	r30
	 
	 reti	/* ����߸��� */