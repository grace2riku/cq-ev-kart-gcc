#include "ad.h"
#include "port.h"

void setFETDrivePattern(void);
void setFETStopPattern(void);

#define PFC5		(*(volatile unsigned char*)0xFFFFF46A)
#define PFCE5		(*(volatile unsigned char*)0xFFFFF70A)
#define PMC5		(*(volatile unsigned char*)0xFFFFF44A)

#define TAB0CTL0	(*(volatile unsigned char*)0xFFFFF540)
#define TAB0CCIC0	(*(volatile unsigned char*)0xFFFFF126)
#define TAB0OVIC	(*(volatile unsigned char*)0xFFFFF124)
#define TAB0CTL1	(*(volatile unsigned char*)0xFFFFF541)
#define TAB0IOC0	(*(volatile unsigned char*)0xFFFFF542)
#define TAB0IOC3	(*(volatile unsigned char*)0xFFFFF562)
#define TAB0OPT0	(*(volatile unsigned char*)0xFFFFF545)
#define TAB0OPT1	(*(volatile unsigned char*)0xFFFFF560)
#define TAB0OPT2	(*(volatile unsigned char*)0xFFFFF561)
#define TAB0CCR0	(*(volatile unsigned short*)0xFFFFF546)
#define TAB0CCR1	(*(volatile unsigned short*)0xFFFFF548)
#define TAB0CCR2	(*(volatile unsigned short*)0xFFFFF54A)
#define TAB0CCR3	(*(volatile unsigned short*)0xFFFFF54C)
#define TAB0DTC		(*(volatile unsigned short*)0xFFFFF564)
#define HZA0CTL0	(*(volatile unsigned char*)0xFFFFF570)

#define MOTOR_CCR0_VALUE	(799)
#define MOTOR_CCR1_VALUE	(800)
#define MOTOR_CCR2_VALUE	(800)
#define MOTOR_CCR3_VALUE	(800)

#define TARGET_DUTY_MAX	(800)	/* 目標PWM MAX */

static unsigned char MotorDrive = 0;	/* モーター駆動中フラグ0：停止中/1：駆動中 */

/* モーター制御機能初期化 */
void MOTOR_init(void){
	/* タイマAB0の動作の制御：内部動作クロック動作禁止 */ 
	TAB0CTL0 &= 0x7F;

	/* INTTAB0CC0割込みマスク */ 
	TAB0CCIC0 |= 0x40;
	/* INTTAB0CC0割込みステータスクリア */ 
	TAB0CCIC0 &= 0x7F;

	/* INTTAB0OV割込みマスク */ 
	TAB0OVIC |= 0x40;
	/* INTTAB0OV割込みステータスクリア */ 
	TAB0OVIC &= 0x7F;

	/* カウントクロックfxp1(=fxx) 32MHz */ 
	TAB0CTL0 = 0x00;
	/* タイマ・モード：三角波PWMモード */ 
	TAB0CTL1 = 0x07;
	/* 
	 * TOAB0m出力レベル：通常出力or反転出力⇒通常出力
	 * TOAB0m出力の設定：タイマ出力禁止
	*/
	TAB0IOC0 = 0x00;
	/* 
	 * TOAB0Bm端子出力レベルの設定：TOAB0Bm端子出力反転禁止
	 * TOAB0Bm端子出力の設定：TOAB0Bm端子出力禁止
	*/
	TAB0IOC3 = 0x00;

	/* */
	TAB0OPT0 = 0x00;
	TAB0OPT1 = 0x00;
	TAB0OPT2 = 0x00;

	/* 周期の設定 */
	TAB0CCR0 = 799;
	/* U相の出力幅 TAB0CCR0+1を設定する */
	TAB0CCR1 = 800;
	/* V相の出力幅 TAB0CCR0+1を設定する */
	TAB0CCR2 = 800;
	/* W相の出力幅 TAB0CCR0+1を設定する */
	TAB0CCR3 = 800;
	/* デッド・タイム値 使用するFETのON時間に依存するパラメータ
	* 2us (1/32MHz)*64
	*/
	TAB0DTC = 64;

	HZA0CTL0 = 0x00;
	/* ハイ・インピーダンス出力制御動作許可 */
	HZA0CTL0 |= 0x80;
	
	/*
	* P5
	* P5.0：FET U相 Hiサイド  TOAB0T1兼用端子に設定
	* P5.1：FET U相 Lowサイド TOAB0B1兼用端子に設定
	* P5.2：FET V相 Hiサイド  TOAB0T2兼用端子に設定
	* P5.3：FET V相 Lowサイド TOAB0B2兼用端子に設定
	* P5.4：FET W相 Hiサイド  TOAB0T3兼用端子に設定
	* P5.5：FET W相 Lowサイド TOAB0T3兼用端子に設定
	*/
	PFC5 |= 0x3F;
	PFCE5 |= 0x3F;
	PMC5 |= 0x3F;
}


void MOTOR_Start(void){
	/* タイマAB0の動作の制御：内部動作クロック動作許可 */ 
	TAB0CTL0 |= 0x80;
}

void calcDriveDuty(const unsigned short accel_value, unsigned short* driveDuty){
	unsigned short duty = 0;
	
	/* アクセルが開いているか */
	if (accel_value > ACCEL_0_AD_VALUE){
		if(accel_value < ACCEL_100_AD_VALUE) {
			/* アクセル開度の比率で目標dutyを計算する */
			duty = (unsigned short)(((unsigned long)(accel_value - ACCEL_0_AD_VALUE) * TARGET_DUTY_MAX) / ACCEL_AD_VALUE_RANGE);
			/* 出力制限 */
			if(duty >= TARGET_DUTY_MAX) {
				*driveDuty = TARGET_DUTY_MAX;
			} else {
				*driveDuty = duty;
			}
		} else {
			/* アクセル100%ポイントよりアクセル開度が大きいときはdutyを100%にする */
			*driveDuty = TARGET_DUTY_MAX;
		}
	} else {
		/* アクセル0%ポイントよりアクセル開度が小さいときはdutyを0%にする */
		*driveDuty = 0;
	}
}



void setDriveDuty(const unsigned short accel_value, const unsigned short driveDuty){
	volatile unsigned short duty = 0;
	
	if (accel_value > ACCEL_0_AD_VALUE){
		if (!MotorDrive) {
			MotorDrive = 1;
			/* モーターが停止中だったら強制的に回転する */
			setFETDrivePattern();
		}
		duty = MOTOR_CCR1_VALUE - driveDuty;
	} else {
		/* 停止処理 */
		MotorDrive = 0;
		duty = MOTOR_CCR1_VALUE;
		setFETStopPattern();
	}
	
	/* dutyをレジスタに設定する */
	TAB0CCR2 = duty;
	TAB0CCR3 = duty;
	TAB0CCR1 = duty;
}


typedef enum _HallSensorPattern {
	Stage1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6,
	ErrorPattern = 0xff
} HallSensorPattern;

	
/* ホールセンサIOポートレベルからモーター転流パターンを取得するテーブル */
const HallSensorPattern HallSensorPatternTable[8] = 
{
	ErrorPattern,
	Stage2,
	Stage4,
	Stage3,
	Stage6,
	Stage1,
	Stage5,
	ErrorPattern,
};

#define FET_UH_PWM	(0x04)
#define FET_VH_PWM	(0x10)
#define FET_WH_PWM	(0x40)
#define FET_UL_ON	(0x08)
#define FET_VL_ON	(0x20)
#define FET_WL_ON	(0x80)
#define FET_OFF		(0x00)

/* 
 * FET転流パターンとLED点灯パターンのテーブル 
 * FET転流パターンはマイコンのモーター制御機能のレジスタ設定値
 * LED点灯パターンはインバーター基板のIOポートデータレジスタ設定値
*/
const unsigned char CommutationPatternTable[6][3] = 
{
	/* 
	 * [stage][0]：FET H側レジスタ設定値 
	 * [stage][1]：FET L側レジスタ設定値
	 * [stage][2]：FET LED IOポートデータレジスタ設定値
	*/
	{FET_UH_PWM, FET_VL_ON, LED_FET_UHVL_ON},
	{FET_UH_PWM, FET_WL_ON, LED_FET_UHWL_ON},
	{FET_VH_PWM, FET_WL_ON, LED_FET_VHWL_ON},
	{FET_VH_PWM, FET_UL_ON, LED_FET_VHUL_ON},
	{FET_WH_PWM, FET_UL_ON, LED_FET_WHUL_ON},
	{FET_WH_PWM, FET_VL_ON, LED_FET_WHVL_ON}
};

void setFETDrivePattern(void) {
	volatile unsigned char hall_level = 0;
	volatile unsigned char stage_index = 0;
	HallSensorPattern stage = ErrorPattern;

	
	hall_level = P9L & 0x1c;
	
	/* ホールセンサのレベルを取得する */
	if (hall_level & HALLSENSOR_U_PORT) stage_index |= 0x01;
	if (hall_level & HALLSENSOR_V_PORT) stage_index |= 0x02;
	if (hall_level & HALLSENSOR_W_PORT) stage_index |= 0x04;

	
	/* ホールセンサーのパターンから転流パターンのステージを取得する */
	stage = HallSensorPatternTable[stage_index];
	
	/* */
	if(MotorDrive && (stage != ErrorPattern) ){
		TAB0CTL0 &= 0x7F;								/* モーター制御機能のタイマ動作禁止 */
		TAB0IOC0 = CommutationPatternTable[stage][0];	/* FET H側転流パターンレジスタ設定 */
		TAB0IOC3 = CommutationPatternTable[stage][1];	/* FET L側転流パターンレジスタ設定 */
		TAB0CTL0 |= 0x80;								/* モーター制御機能のタイマ動作許可 */
		
		/* LED点灯パターン設定 */
		PORT_LED_FET = CommutationPatternTable[stage][2];
	} else {
		/* ホールセンサーパターン異常のため通電停止 */
		setFETStopPattern();
	}
	
}

void setFETStopPattern(void){
	/* モーター制御機能のタイマ動作禁止 */
	TAB0CTL0 &= 0x7F;
	
	TAB0IOC0 = FET_OFF;
	TAB0IOC3 = FET_OFF;

	/* モーター制御機能のタイマ動作許可 */
	TAB0CTL0 |= 0x80;

	/* LED点灯パターン設定 */
	PORT_LED_FET = LED_FET_ALL_OFF;
}

