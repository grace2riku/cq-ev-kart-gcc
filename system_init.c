#include "port.h"
#include "timer.h"
#include "ad.h"
#include "motor.h"
#include "serial.h"

void system_init(void){
	/* IO�|�[�g�̏����� */
	port_init();
	
	/* UART�̏����� */
	UARTD1_init();
	
	/* AD�R���o�[�^�[�̏����� */
	AD0_init();
	
	/* �^�C�}�[�̏����� */
	TMM0_init();
	TAB1_init();

	/* ���[�^�[�@�\�֘A�̏����� */
	MOTOR_init();
	
	/* �����݋��� */
	__asm("ei");
}

