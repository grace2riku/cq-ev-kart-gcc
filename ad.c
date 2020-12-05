#include "ad.h"

#define ADA0M0		(*(volatile unsigned char*)0xFFFFF200)
#define ADA0M1		(*(volatile unsigned char*)0xFFFFF201)
#define ADA0M2		(*(volatile unsigned char*)0xFFFFF203)
#define ADIC		(*(volatile unsigned char*)0xFFFFF164)
#define ADA0S		(*(volatile unsigned char*)0xFFFFF202)
#define ADA0CR0		(*(volatile unsigned short*)0xFFFFF210)

#define PMC7L 	(*(volatile unsigned char*)0xFFFFF44E)


/* ADコンバータ初期化 */
void AD0_init(void){
	/* AD動作停止 */ 
	ADA0M0 = 0x00;
	/* AD割込みマスク */
	ADIC |= 0x40;
	/* AD変換割込みステータスクリア */
	ADIC &= 0x7F;
	/* 
	* A/D変換制御：AD電源ON
	* A/Dコンバータ動作モードを指定：連続スキャンモード
	* 外部トリガ（ADTRG端子）入力の有効エッジ：エッジ検出なし
	* トリガ・モード：ソフトウェアトリガ
	*/
	ADA0M0 = 0x50;
	/* AD変換時間：4us 128/fxp1(32MHz) */
	ADA0M1 = 0x03;
	/* 
	* 診断機能：禁止
	* ディスチャージ機能：禁止
	*/
	ADA0M2 = 0x00;
	/* A/D変換するアナログ電圧の入力ポート：スキャン・モードでANI0 */
	ADA0S = 0x00;

	/*
	* P7L
	* P7L.0：アクセル入力電圧 ANI0兼用端子に設定
	*/
	PMC7L |= 0x01;
}


void get_ad_value(unsigned short* ad_buffer) {
	/* AD動作停止 */ 
	ADA0M0 &= 0x7F;
	/* AD変換割込みステータスクリア */
	ADIC &= 0x7F;
	
	/* A/D変換するアナログ電圧の入力ポート：スキャン・モードでANI0 */
	ADA0S = 0x00;
	
	/* AD動作許可 */ 
	ADA0M0 |= 0x80;

	/* AD変換割込みステータスになるまで待つ */
	while( (ADIC & 0x80) == 0 ) {}

	/* AD動作停止 */ 
	ADA0M0 &= 0x7F;
	/* AD変換割込みステータスクリア */
	ADIC &= 0x7F;

	*ad_buffer = (unsigned short)(ADA0CR0 >> 6);
}


int isAccelOff(void) {
	int ret = 0;
	unsigned short accel_value = 0;
	
	/* アクセル値取得 */
	get_ad_value(&accel_value);
	
	/* アクセルOFFか確認する */
	if(accel_value < ACCEL_0_AD_VALUE) {
		ret = 1;
	}
	
	return ret;
}
