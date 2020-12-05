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
	
	/* 起動時アクセルOFF確認後にモーター回転制御可能とする */
	while( !isAccelOff() );

	PORT_LED_FET = LED_FET_ALL_OFF;
	
	/* タイマカウントスタート */
	TMM0_start();
	TAB1_start();
	MOTOR_Start();

	UARTD1_Start();
	
	while (1) {
		/* モーター制御周期(2ms経過)か確認する */
		if ( get_motor_control_cyclic_flag() ) {
			/* アクセルAD値取得 */
			get_ad_value(&accel_value);
			
			/* 目標Duty計算 */
			calcDriveDuty(accel_value, &drive_duty);
			
			/* 駆動Duty設定 */
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
