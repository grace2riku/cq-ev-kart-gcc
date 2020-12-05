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

#define TARGET_DUTY_MAX	(800)	/* �ڕWPWM MAX */

static unsigned char MotorDrive = 0;	/* ���[�^�[�쓮���t���O0�F��~��/1�F�쓮�� */

/* ���[�^�[����@�\������ */
void MOTOR_init(void){
	/* �^�C�}AB0�̓���̐���F��������N���b�N����֎~ */ 
	TAB0CTL0 &= 0x7F;

	/* INTTAB0CC0�����݃}�X�N */ 
	TAB0CCIC0 |= 0x40;
	/* INTTAB0CC0�����݃X�e�[�^�X�N���A */ 
	TAB0CCIC0 &= 0x7F;

	/* INTTAB0OV�����݃}�X�N */ 
	TAB0OVIC |= 0x40;
	/* INTTAB0OV�����݃X�e�[�^�X�N���A */ 
	TAB0OVIC &= 0x7F;

	/* �J�E���g�N���b�Nfxp1(=fxx) 32MHz */ 
	TAB0CTL0 = 0x00;
	/* �^�C�}�E���[�h�F�O�p�gPWM���[�h */ 
	TAB0CTL1 = 0x07;
	/* 
	 * TOAB0m�o�̓��x���F�ʏ�o��or���]�o�́˒ʏ�o��
	 * TOAB0m�o�͂̐ݒ�F�^�C�}�o�͋֎~
	*/
	TAB0IOC0 = 0x00;
	/* 
	 * TOAB0Bm�[�q�o�̓��x���̐ݒ�FTOAB0Bm�[�q�o�͔��]�֎~
	 * TOAB0Bm�[�q�o�͂̐ݒ�FTOAB0Bm�[�q�o�͋֎~
	*/
	TAB0IOC3 = 0x00;

	/* */
	TAB0OPT0 = 0x00;
	TAB0OPT1 = 0x00;
	TAB0OPT2 = 0x00;

	/* �����̐ݒ� */
	TAB0CCR0 = 799;
	/* U���̏o�͕� TAB0CCR0+1��ݒ肷�� */
	TAB0CCR1 = 800;
	/* V���̏o�͕� TAB0CCR0+1��ݒ肷�� */
	TAB0CCR2 = 800;
	/* W���̏o�͕� TAB0CCR0+1��ݒ肷�� */
	TAB0CCR3 = 800;
	/* �f�b�h�E�^�C���l �g�p����FET��ON���ԂɈˑ�����p�����[�^
	* 2us (1/32MHz)*64
	*/
	TAB0DTC = 64;

	HZA0CTL0 = 0x00;
	/* �n�C�E�C���s�[�_���X�o�͐��䓮�싖�� */
	HZA0CTL0 |= 0x80;
	
	/*
	* P5
	* P5.0�FFET U�� Hi�T�C�h  TOAB0T1���p�[�q�ɐݒ�
	* P5.1�FFET U�� Low�T�C�h TOAB0B1���p�[�q�ɐݒ�
	* P5.2�FFET V�� Hi�T�C�h  TOAB0T2���p�[�q�ɐݒ�
	* P5.3�FFET V�� Low�T�C�h TOAB0B2���p�[�q�ɐݒ�
	* P5.4�FFET W�� Hi�T�C�h  TOAB0T3���p�[�q�ɐݒ�
	* P5.5�FFET W�� Low�T�C�h TOAB0T3���p�[�q�ɐݒ�
	*/
	PFC5 |= 0x3F;
	PFCE5 |= 0x3F;
	PMC5 |= 0x3F;
}


void MOTOR_Start(void){
	/* �^�C�}AB0�̓���̐���F��������N���b�N���싖�� */ 
	TAB0CTL0 |= 0x80;
}

void calcDriveDuty(const unsigned short accel_value, unsigned short* driveDuty){
	unsigned short duty = 0;
	
	/* �A�N�Z�����J���Ă��邩 */
	if (accel_value > ACCEL_0_AD_VALUE){
		if(accel_value < ACCEL_100_AD_VALUE) {
			/* �A�N�Z���J�x�̔䗦�ŖڕWduty���v�Z���� */
			duty = (unsigned short)(((unsigned long)(accel_value - ACCEL_0_AD_VALUE) * TARGET_DUTY_MAX) / ACCEL_AD_VALUE_RANGE);
			/* �o�͐��� */
			if(duty >= TARGET_DUTY_MAX) {
				*driveDuty = TARGET_DUTY_MAX;
			} else {
				*driveDuty = duty;
			}
		} else {
			/* �A�N�Z��100%�|�C���g���A�N�Z���J�x���傫���Ƃ���duty��100%�ɂ��� */
			*driveDuty = TARGET_DUTY_MAX;
		}
	} else {
		/* �A�N�Z��0%�|�C���g���A�N�Z���J�x���������Ƃ���duty��0%�ɂ��� */
		*driveDuty = 0;
	}
}



void setDriveDuty(const unsigned short accel_value, const unsigned short driveDuty){
	volatile unsigned short duty = 0;
	
	if (accel_value > ACCEL_0_AD_VALUE){
		if (!MotorDrive) {
			MotorDrive = 1;
			/* ���[�^�[����~���������狭���I�ɉ�]���� */
			setFETDrivePattern();
		}
		duty = MOTOR_CCR1_VALUE - driveDuty;
	} else {
		/* ��~���� */
		MotorDrive = 0;
		duty = MOTOR_CCR1_VALUE;
		setFETStopPattern();
	}
	
	/* duty�����W�X�^�ɐݒ肷�� */
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

	
/* �z�[���Z���TIO�|�[�g���x�����烂�[�^�[�]���p�^�[�����擾����e�[�u�� */
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
 * FET�]���p�^�[����LED�_���p�^�[���̃e�[�u�� 
 * FET�]���p�^�[���̓}�C�R���̃��[�^�[����@�\�̃��W�X�^�ݒ�l
 * LED�_���p�^�[���̓C���o�[�^�[���IO�|�[�g�f�[�^���W�X�^�ݒ�l
*/
const unsigned char CommutationPatternTable[6][3] = 
{
	/* 
	 * [stage][0]�FFET H�����W�X�^�ݒ�l 
	 * [stage][1]�FFET L�����W�X�^�ݒ�l
	 * [stage][2]�FFET LED IO�|�[�g�f�[�^���W�X�^�ݒ�l
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
	
	/* �z�[���Z���T�̃��x�����擾���� */
	if (hall_level & HALLSENSOR_U_PORT) stage_index |= 0x01;
	if (hall_level & HALLSENSOR_V_PORT) stage_index |= 0x02;
	if (hall_level & HALLSENSOR_W_PORT) stage_index |= 0x04;

	
	/* �z�[���Z���T�[�̃p�^�[������]���p�^�[���̃X�e�[�W���擾���� */
	stage = HallSensorPatternTable[stage_index];
	
	/* */
	if(MotorDrive && (stage != ErrorPattern) ){
		TAB0CTL0 &= 0x7F;								/* ���[�^�[����@�\�̃^�C�}����֎~ */
		TAB0IOC0 = CommutationPatternTable[stage][0];	/* FET H���]���p�^�[�����W�X�^�ݒ� */
		TAB0IOC3 = CommutationPatternTable[stage][1];	/* FET L���]���p�^�[�����W�X�^�ݒ� */
		TAB0CTL0 |= 0x80;								/* ���[�^�[����@�\�̃^�C�}���싖�� */
		
		/* LED�_���p�^�[���ݒ� */
		PORT_LED_FET = CommutationPatternTable[stage][2];
	} else {
		/* �z�[���Z���T�[�p�^�[���ُ�̂��ߒʓd��~ */
		setFETStopPattern();
	}
	
}

void setFETStopPattern(void){
	/* ���[�^�[����@�\�̃^�C�}����֎~ */
	TAB0CTL0 &= 0x7F;
	
	TAB0IOC0 = FET_OFF;
	TAB0IOC3 = FET_OFF;

	/* ���[�^�[����@�\�̃^�C�}���싖�� */
	TAB0CTL0 |= 0x80;

	/* LED�_���p�^�[���ݒ� */
	PORT_LED_FET = LED_FET_ALL_OFF;
}

