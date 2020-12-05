#include "port.h"
extern void setFETDrivePattern(void);

#define TM0CTL0 	(*(volatile unsigned char*)0xFFFFF690)
#define TM0EQIC0 	(*(volatile unsigned char*)0xFFFFF14C)
#define TM0CMP0 	(*(volatile unsigned short*)0xFFFFF694)

static unsigned char motor_control_cyclic_flag = 0;

/* TMM0：モータの制御周期生成で使用 */
void TMM0_init(void){
	/* タイマM0の動作の制御：内部動作クロック動作禁止 */
	TM0CTL0 &= 0x7F;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込みマスクフラグ：割込み禁止 */
	TM0EQIC0 |= 0x40;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込み要求フラグ：割込み要求なしにクリア */
	TM0EQIC0 &= 0x7F;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込み優先順位：レベル7(最低優先順位) */
	TM0EQIC0 |= 0x07;
	/* 
	* オプションバイト0x7B ビット2 PRSI=0：周辺クロックfxp1とfxp2の分周設定 fxp1, fxp2 = fxx
	*/
	TM0CTL0 = 0x00;
	/* TMM0コンペア・レジスタ0：2ms (1/32MHz) * (64000 - 1) */
	TM0CMP0 = 0xf9ff;
}


void TMM0_start(void) {
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込み要求フラグ：割込み要求なしにクリア */
	TM0EQIC0 &= 0x7F;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込みマスクフラグ：割込み許可 */
	TM0EQIC0 &= 0xBF;
	/* タイマM0の動作の制御：内部動作クロック動作許可 */
	TM0CTL0 |= 0x80;
}


void TMM0_stop(void) {
	/* タイマM0の動作の制御：内部動作クロック動作禁止 */
	TM0CTL0 &= 0x7F;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込みマスクフラグ：割込み禁止 */
	TM0EQIC0 |= 0x40;
	/* 割込みINTTM0EQ0(TMM0コンペア一致)割込み要求フラグ：割込み要求なしにクリア */
	TM0EQIC0 &= 0x7F;
}

void clear_motor_control_cyclic_flag(){
	motor_control_cyclic_flag = 0;
}

unsigned char get_motor_control_cyclic_flag(void){
	return motor_control_cyclic_flag;
}

void _interrupt_tm0eq0_handler(void)
{
	motor_control_cyclic_flag = 1;
}


#define TAB1CTL0 	(*(volatile unsigned char*)0xFFFFF610)
#define TAB1CCIC0 	(*(volatile unsigned char*)0xFFFFF188)
#define TAB1CCIC1 	(*(volatile unsigned char*)0xFFFFF18A)
#define TAB1CCIC2 	(*(volatile unsigned char*)0xFFFFF18C)
#define TAB1CCIC3 	(*(volatile unsigned char*)0xFFFFF18E)
#define TAB1OVIC 	(*(volatile unsigned char*)0xFFFFF186)
#define TAB1CTL1 	(*(volatile unsigned char*)0xFFFFF611)
#define TAB1OPT0 	(*(volatile unsigned char*)0xFFFFF615)
#define TAB1IOC1 	(*(volatile unsigned char*)0xFFFFF613)
#define TAB1CCR0 	(*(volatile unsigned short*)0xFFFFF616)
#define PFC9L	 	(*(volatile unsigned char*)0xFFFFF472)
#define PFCE9L	 	(*(volatile unsigned char*)0xFFFFF712)
#define PMC9L	 	(*(volatile unsigned char*)0xFFFFF452)


/* TAB1：ホールセンサ割込みで利用する */
void TAB1_init(void){
	/* タイマAB1の動作の制御：内部動作クロック動作禁止 */
	TAB1CTL0 &= 0x7F;

	/* INTTAB1CC0割込み禁止 */
	TAB1CCIC0 |= 0x40;
	/* INTTAB1CC0割込み要求クリア */
	TAB1CCIC0 &= 0x7F;

	/* INTTAB1CC1割込み禁止 */
	TAB1CCIC1 |= 0x40;
	/* INTTAB1CC1割込み要求クリア */
	TAB1CCIC1 &= 0x7F;

	/* INTTAB1CC2割込み禁止 */
	TAB1CCIC2 |= 0x40;
	/* INTTAB1CC2割込み要求クリア */
	TAB1CCIC2 &= 0x7F;

	/* INTTAB1CC3割込み禁止 */
	TAB1CCIC3 |= 0x40;
	/* INTTAB1CC2割込み要求クリア */
	TAB1CCIC3 &= 0x7F;

	/* INTTAB1OV割込み禁止 */
	TAB1OVIC |= 0x40;
	/* INTTAB1OV割込み要求クリア */
	TAB1OVIC &= 0x7F;

	/* INTTAB1CC1割込み優先順位：7 */
	TAB1CCIC1 |= 0x07;
	/* INTTAB1CC2割込み優先順位：7 */
	TAB1CCIC2 |= 0x07;
	/* INTTAB1CC3割込み優先順位：7 */
	TAB1CCIC3 |= 0x07;

	/* ホールセンサエッジ切り替わりで回転数計測に使用する */
	/* 内部カウントクロック：fxp1(32MHz)/32=1MHz */
	TAB1CTL0 = 0x05;
	/* 内部カウント/フリーランニング動作 */
	TAB1CTL1 = 0x05;
	/* CCR3,CCR2,CCR1キャプチャレジスタとして使用 */
	TAB1OPT0 = 0x80 | 0x40 | 0x20;
	/* TIAB13,TIAB12,TIAB11 キャプチャ有効エッジ：両エッジ */
	TAB1IOC1 = 0xc0 | 0x30 | 0x0c;
	/* 特に使用しないが0に設定 */
/*	TAB1CCR0 = 0; */
	
	/* ホールセンサU相 TIAB11(P92)兼用端子設定 */
	PFC9L |= 0x04;
	PFCE9L &= 0xFB;
	PMC9L |= 0x04;
	
	/* ホールセンサV相 TIAB12(P93)兼用端子設定 */
	PFC9L |= 0x08;
	PFCE9L &= 0xF7;
	PMC9L |= 0x08;

	/* ホールセンサW相 TIAB13(P94)兼用端子設定 */
	PFC9L |= 0x10;
	PFCE9L &= 0xEF;
	PMC9L |= 0x10;
}


void TAB1_start(void){
	/* INTTAB1CC1割込み要求クリア */
	TAB1CCIC1 &= 0x7F;
	/* INTTAB1CC1割込み許可 */
	TAB1CCIC1 &= 0xBF;
	
	/* INTTAB1CC2割込み要求クリア */
	TAB1CCIC2 &= 0x7F;
	/* INTTAB1CC1割込み許可 */
	TAB1CCIC2 &= 0xBF;

	/* INTTAB1CC3割込み要求クリア */
	TAB1CCIC3 &= 0x7F;
	/* INTTAB1CC3割込み許可 */
	TAB1CCIC3 &= 0xBF;

	/* タイマAB1の動作の制御：内部動作クロック動作許可 */
	TAB1CTL0 |= 0x80;
}

void TAB1_stop(void){
	/* タイマAB1の動作の制御：内部動作クロック動作禁止 */
	TAB1CTL0 &= 0x7F;

	/* INTTAB1CC1割込み禁止 */
	TAB1CCIC1 |= 0x40;
	/* INTTAB1CC1割込み要求クリア */
	TAB1CCIC1 &= 0x7F;

	/* INTTAB1CC2割込み禁止 */
	TAB1CCIC2 |= 0x40;
	/* INTTAB1CC1割込み要求クリア */
	TAB1CCIC2 &= 0x7F;

	/* INTTAB1CC3割込み禁止 */
	TAB1CCIC3 |= 0x40;
	/* INTTAB1CC3割込み要求クリア */
	TAB1CCIC3 &= 0x7F;
}

void _interrupt_hall_sensor_u(void) {
	/* タイマキャプチャ値を取得 */
	
	/* タイマキャプチャ値からモーター回転数を計算する */

	/* 転流パターン設定 */
	setFETDrivePattern();

	/* ホールセンサLED点灯・消灯 */
	if(PORT_HALLSENSOR & HALLSENSOR_U_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_U_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_U_BIT;
}

void _interrupt_hall_sensor_v(void) {
	/* タイマキャプチャ値を取得 */
	
	/* タイマキャプチャ値からモーター回転数を計算する */

	/* 転流パターン設定 */
	setFETDrivePattern();
	
	/* ホールセンサLED点灯・消灯 */
	if(PORT_HALLSENSOR & HALLSENSOR_V_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_V_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_V_BIT;
}

void _interrupt_hall_sensor_w(void) {
	/* タイマキャプチャ値を取得 */
	
	/* タイマキャプチャ値からモーター回転数を計算する */

	/* 転流パターン設定 */
	setFETDrivePattern();

	/* ホールセンサLED点灯・消灯 */
	if(PORT_HALLSENSOR & HALLSENSOR_W_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_W_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_W_BIT;
}
