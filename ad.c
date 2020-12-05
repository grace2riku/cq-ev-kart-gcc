#include "ad.h"

#define ADA0M0		(*(volatile unsigned char*)0xFFFFF200)
#define ADA0M1		(*(volatile unsigned char*)0xFFFFF201)
#define ADA0M2		(*(volatile unsigned char*)0xFFFFF203)
#define ADIC		(*(volatile unsigned char*)0xFFFFF164)
#define ADA0S		(*(volatile unsigned char*)0xFFFFF202)
#define ADA0CR0		(*(volatile unsigned short*)0xFFFFF210)

#define PMC7L 	(*(volatile unsigned char*)0xFFFFF44E)


/* AD�R���o�[�^������ */
void AD0_init(void){
	/* AD�����~ */ 
	ADA0M0 = 0x00;
	/* AD�����݃}�X�N */
	ADIC |= 0x40;
	/* AD�ϊ������݃X�e�[�^�X�N���A */
	ADIC &= 0x7F;
	/* 
	* A/D�ϊ�����FAD�d��ON
	* A/D�R���o�[�^���샂�[�h���w��F�A���X�L�������[�h
	* �O���g���K�iADTRG�[�q�j���̗͂L���G�b�W�F�G�b�W���o�Ȃ�
	* �g���K�E���[�h�F�\�t�g�E�F�A�g���K
	*/
	ADA0M0 = 0x50;
	/* AD�ϊ����ԁF4us 128/fxp1(32MHz) */
	ADA0M1 = 0x03;
	/* 
	* �f�f�@�\�F�֎~
	* �f�B�X�`���[�W�@�\�F�֎~
	*/
	ADA0M2 = 0x00;
	/* A/D�ϊ�����A�i���O�d���̓��̓|�[�g�F�X�L�����E���[�h��ANI0 */
	ADA0S = 0x00;

	/*
	* P7L
	* P7L.0�F�A�N�Z�����͓d�� ANI0���p�[�q�ɐݒ�
	*/
	PMC7L |= 0x01;
}


void get_ad_value(unsigned short* ad_buffer) {
	/* AD�����~ */ 
	ADA0M0 &= 0x7F;
	/* AD�ϊ������݃X�e�[�^�X�N���A */
	ADIC &= 0x7F;
	
	/* A/D�ϊ�����A�i���O�d���̓��̓|�[�g�F�X�L�����E���[�h��ANI0 */
	ADA0S = 0x00;
	
	/* AD���싖�� */ 
	ADA0M0 |= 0x80;

	/* AD�ϊ������݃X�e�[�^�X�ɂȂ�܂ő҂� */
	while( (ADIC & 0x80) == 0 ) {}

	/* AD�����~ */ 
	ADA0M0 &= 0x7F;
	/* AD�ϊ������݃X�e�[�^�X�N���A */
	ADIC &= 0x7F;

	*ad_buffer = (unsigned short)(ADA0CR0 >> 6);
}


int isAccelOff(void) {
	int ret = 0;
	unsigned short accel_value = 0;
	
	/* �A�N�Z���l�擾 */
	get_ad_value(&accel_value);
	
	/* �A�N�Z��OFF���m�F���� */
	if(accel_value < ACCEL_0_AD_VALUE) {
		ret = 1;
	}
	
	return ret;
}
