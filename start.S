/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  Copyright (C) 2005 by Freelines CO.,Ltd
 * 
 *  Copyright (C) 2010 by Meika Sugimoto
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 */
 
#include "v850asm.inc"
#define STACKTOP			(0x03FFEFF0)
#define VSWC				(0xFFFFF06E)

/* C言語関数 */
	.extern _main
	.extern	_clock_init
	.extern	_system_init

	.section	".text", "ax"
	.align	4
		
	.globl _start
	.globl _startend
_start:
	di
	
	/* 各種ポインタの初期化 */
	Lea		STACKTOP, r3
	Lea		__text , tp
	Lea		__ep , ep
	Lea 	__gp , gp
	Lea		__ctbp , r6
	ldsr	r6 , 20

	/* 内蔵周辺I/Oレジスタに対するバス・アクセスのウエイトを制御する 
	 * 動作周波数fcpu 25 MHz≦fCPU<33.3 MHz のため、11Hを設定しウェイト数2に設定する。
	 */
	movea	0x11, r0, r12
	mov		VSWC, r13
	st.b	r12, 0[r13]
	
	/* bssセクションのクリア */
	Lea		__bss_start, ep
	Lea		__bss_end, r6
bss_clear_start:
	cmp		ep, r6
	be		bss_clear_end
	sst.w	r0, 0[ep]
	add		4, ep
	br		bss_clear_start
bss_clear_end:
	
	/* dataセクションの初期化(ROM化対応)
	 * __idata_startから__idata_endまでを__data_start以降にコピーする 
	*/
	Lea		__idata_start, r6
	Lea		__idata_end,   r7
	Lea		__data_start,  r8
data_init_start:
	cmp		r6, r7
	be		data_init_end
	ld.w	0[r6], r9
	st.w	r9, 0[r8]
	add		4, r6
	add		4, r8
	br		data_init_start
data_init_end:
	
	/* クロックの初期化 */
	jarl	_clock_init,	lp
	
	/* システムの初期化 */
	jarl	_system_init,	lp
	
	/* main関数へジャンプ */
	jarl	_main,lp			-- call main function
	halt

	.end
_startend