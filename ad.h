
#ifndef _AD_
#define _AD_

#define ACCEL_0_AD_VALUE 		(205)	/* ���[�^�[�쓮�J�n�A�N�Z���|�C���g0%AD�l �d���l���Z1.0V */ 
#define ACCEL_100_AD_VALUE		(819)	/* ���[�^�[�쓮�J�n�A�N�Z���|�C���g100%AD�l �d���l���Z4.0V */ 
#define ACCEL_AD_VALUE_RANGE	(614)	/* �A�N�Z���|�C���g�̕� */ 


void AD0_init(void);
int isAccelOff(void);
void get_ad_value(unsigned short* ad_buffer);

#endif