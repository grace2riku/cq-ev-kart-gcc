
#ifndef _AD_
#define _AD_

#define ACCEL_0_AD_VALUE 		(205)	/* モーター駆動開始アクセルポイント0%AD値 電圧値換算1.0V */ 
#define ACCEL_100_AD_VALUE		(819)	/* モーター駆動開始アクセルポイント100%AD値 電圧値換算4.0V */ 
#define ACCEL_AD_VALUE_RANGE	(614)	/* アクセルポイントの幅 */ 


void AD0_init(void);
int isAccelOff(void);
void get_ad_value(unsigned short* ad_buffer);

#endif