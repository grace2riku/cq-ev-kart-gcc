#include "port.h"
extern void setFETDrivePattern(void);

#define TM0CTL0 	(*(volatile unsigned char*)0xFFFFF690)
#define TM0EQIC0 	(*(volatile unsigned char*)0xFFFFF14C)
#define TM0CMP0 	(*(volatile unsigned short*)0xFFFFF694)

static unsigned char motor_control_cyclic_flag = 0;

/* TMM0�F���[�^�̐�����������Ŏg�p */
void TMM0_init(void){
	/* �^�C�}M0�̓���̐���F��������N���b�N����֎~ */
	TM0CTL0 &= 0x7F;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����݃}�X�N�t���O�F�����݋֎~ */
	TM0EQIC0 |= 0x40;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����ݗv���t���O�F�����ݗv���Ȃ��ɃN���A */
	TM0EQIC0 &= 0x7F;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����ݗD�揇�ʁF���x��7(�Œ�D�揇��) */
	TM0EQIC0 |= 0x07;
	/* 
	* �I�v�V�����o�C�g0x7B �r�b�g2 PRSI=0�F���ӃN���b�Nfxp1��fxp2�̕����ݒ� fxp1, fxp2 = fxx
	*/
	TM0CTL0 = 0x00;
	/* TMM0�R���y�A�E���W�X�^0�F2ms (1/32MHz) * (64000 - 1) */
	TM0CMP0 = 0xf9ff;
}


void TMM0_start(void) {
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����ݗv���t���O�F�����ݗv���Ȃ��ɃN���A */
	TM0EQIC0 &= 0x7F;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����݃}�X�N�t���O�F�����݋��� */
	TM0EQIC0 &= 0xBF;
	/* �^�C�}M0�̓���̐���F��������N���b�N���싖�� */
	TM0CTL0 |= 0x80;
}


void TMM0_stop(void) {
	/* �^�C�}M0�̓���̐���F��������N���b�N����֎~ */
	TM0CTL0 &= 0x7F;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����݃}�X�N�t���O�F�����݋֎~ */
	TM0EQIC0 |= 0x40;
	/* ������INTTM0EQ0(TMM0�R���y�A��v)�����ݗv���t���O�F�����ݗv���Ȃ��ɃN���A */
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


/* TAB1�F�z�[���Z���T�����݂ŗ��p���� */
void TAB1_init(void){
	/* �^�C�}AB1�̓���̐���F��������N���b�N����֎~ */
	TAB1CTL0 &= 0x7F;

	/* INTTAB1CC0�����݋֎~ */
	TAB1CCIC0 |= 0x40;
	/* INTTAB1CC0�����ݗv���N���A */
	TAB1CCIC0 &= 0x7F;

	/* INTTAB1CC1�����݋֎~ */
	TAB1CCIC1 |= 0x40;
	/* INTTAB1CC1�����ݗv���N���A */
	TAB1CCIC1 &= 0x7F;

	/* INTTAB1CC2�����݋֎~ */
	TAB1CCIC2 |= 0x40;
	/* INTTAB1CC2�����ݗv���N���A */
	TAB1CCIC2 &= 0x7F;

	/* INTTAB1CC3�����݋֎~ */
	TAB1CCIC3 |= 0x40;
	/* INTTAB1CC2�����ݗv���N���A */
	TAB1CCIC3 &= 0x7F;

	/* INTTAB1OV�����݋֎~ */
	TAB1OVIC |= 0x40;
	/* INTTAB1OV�����ݗv���N���A */
	TAB1OVIC &= 0x7F;

	/* INTTAB1CC1�����ݗD�揇�ʁF7 */
	TAB1CCIC1 |= 0x07;
	/* INTTAB1CC2�����ݗD�揇�ʁF7 */
	TAB1CCIC2 |= 0x07;
	/* INTTAB1CC3�����ݗD�揇�ʁF7 */
	TAB1CCIC3 |= 0x07;

	/* �z�[���Z���T�G�b�W�؂�ւ��ŉ�]���v���Ɏg�p���� */
	/* �����J�E���g�N���b�N�Ffxp1(32MHz)/32=1MHz */
	TAB1CTL0 = 0x05;
	/* �����J�E���g/�t���[�����j���O���� */
	TAB1CTL1 = 0x05;
	/* CCR3,CCR2,CCR1�L���v�`�����W�X�^�Ƃ��Ďg�p */
	TAB1OPT0 = 0x80 | 0x40 | 0x20;
	/* TIAB13,TIAB12,TIAB11 �L���v�`���L���G�b�W�F���G�b�W */
	TAB1IOC1 = 0xc0 | 0x30 | 0x0c;
	/* ���Ɏg�p���Ȃ���0�ɐݒ� */
/*	TAB1CCR0 = 0; */
	
	/* �z�[���Z���TU�� TIAB11(P92)���p�[�q�ݒ� */
	PFC9L |= 0x04;
	PFCE9L &= 0xFB;
	PMC9L |= 0x04;
	
	/* �z�[���Z���TV�� TIAB12(P93)���p�[�q�ݒ� */
	PFC9L |= 0x08;
	PFCE9L &= 0xF7;
	PMC9L |= 0x08;

	/* �z�[���Z���TW�� TIAB13(P94)���p�[�q�ݒ� */
	PFC9L |= 0x10;
	PFCE9L &= 0xEF;
	PMC9L |= 0x10;
}


void TAB1_start(void){
	/* INTTAB1CC1�����ݗv���N���A */
	TAB1CCIC1 &= 0x7F;
	/* INTTAB1CC1�����݋��� */
	TAB1CCIC1 &= 0xBF;
	
	/* INTTAB1CC2�����ݗv���N���A */
	TAB1CCIC2 &= 0x7F;
	/* INTTAB1CC1�����݋��� */
	TAB1CCIC2 &= 0xBF;

	/* INTTAB1CC3�����ݗv���N���A */
	TAB1CCIC3 &= 0x7F;
	/* INTTAB1CC3�����݋��� */
	TAB1CCIC3 &= 0xBF;

	/* �^�C�}AB1�̓���̐���F��������N���b�N���싖�� */
	TAB1CTL0 |= 0x80;
}

void TAB1_stop(void){
	/* �^�C�}AB1�̓���̐���F��������N���b�N����֎~ */
	TAB1CTL0 &= 0x7F;

	/* INTTAB1CC1�����݋֎~ */
	TAB1CCIC1 |= 0x40;
	/* INTTAB1CC1�����ݗv���N���A */
	TAB1CCIC1 &= 0x7F;

	/* INTTAB1CC2�����݋֎~ */
	TAB1CCIC2 |= 0x40;
	/* INTTAB1CC1�����ݗv���N���A */
	TAB1CCIC2 &= 0x7F;

	/* INTTAB1CC3�����݋֎~ */
	TAB1CCIC3 |= 0x40;
	/* INTTAB1CC3�����ݗv���N���A */
	TAB1CCIC3 &= 0x7F;
}

void _interrupt_hall_sensor_u(void) {
	/* �^�C�}�L���v�`���l���擾 */
	
	/* �^�C�}�L���v�`���l���烂�[�^�[��]�����v�Z���� */

	/* �]���p�^�[���ݒ� */
	setFETDrivePattern();

	/* �z�[���Z���TLED�_���E���� */
	if(PORT_HALLSENSOR & HALLSENSOR_U_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_U_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_U_BIT;
}

void _interrupt_hall_sensor_v(void) {
	/* �^�C�}�L���v�`���l���擾 */
	
	/* �^�C�}�L���v�`���l���烂�[�^�[��]�����v�Z���� */

	/* �]���p�^�[���ݒ� */
	setFETDrivePattern();
	
	/* �z�[���Z���TLED�_���E���� */
	if(PORT_HALLSENSOR & HALLSENSOR_V_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_V_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_V_BIT;
}

void _interrupt_hall_sensor_w(void) {
	/* �^�C�}�L���v�`���l���擾 */
	
	/* �^�C�}�L���v�`���l���烂�[�^�[��]�����v�Z���� */

	/* �]���p�^�[���ݒ� */
	setFETDrivePattern();

	/* �z�[���Z���TLED�_���E���� */
	if(PORT_HALLSENSOR & HALLSENSOR_W_PORT) PORT_LED_HALLSENSOR |= LED_HALLSENSOR_W_BIT;
	else PORT_LED_HALLSENSOR &= LED_HALLSENSOR_W_BIT;
}
