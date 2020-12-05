#include <stdio.h>
#include <string.h>
#include "port.h"
#include "timer.h"
#include "ad.h"
#include "motor.h"
#include "serial.h"

char logMessageBuffer[128] = {0};


int main(){
	unsigned short accel_value = 0;
	unsigned short drive_duty = 0;
	unsigned short log_send_count = 0;

	
	PORT_LED_FET = LED_FET_ALL_ON;
	
	/* �N�����A�N�Z��OFF�m�F��Ƀ��[�^�[��]����\�Ƃ��� */
	while( !isAccelOff() );

	PORT_LED_FET = LED_FET_ALL_OFF;
	
	/* �^�C�}�J�E���g�X�^�[�g */
	TMM0_start();
	TAB1_start();
	MOTOR_Start();

	UARTD1_Start();
	
	while (1) {
		/* ���[�^�[�������(2ms�o��)���m�F���� */
		if ( get_motor_control_cyclic_flag() ) {
			/* �A�N�Z��AD�l�擾 */
			get_ad_value(&accel_value);
			
			/* �ڕWDuty�v�Z */
			calcDriveDuty(accel_value, &drive_duty);
			
			/* �쓮Duty�ݒ� */
			setDriveDuty(accel_value, drive_duty);

			clear_motor_control_cyclic_flag();

			++log_send_count;
		}
				
		if (log_send_count >= 500) {
			memset(logMessageBuffer, 0x00, sizeof(logMessageBuffer));
			sprintf(logMessageBuffer, "Duty=%d, ThV(V)=%d\r\n", drive_duty, accel_value);
			UARTD1_SendData((unsigned char*)logMessageBuffer, strlen(logMessageBuffer));

			log_send_count = 0;
		}
		
	}
}
