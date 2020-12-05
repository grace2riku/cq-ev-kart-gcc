#include "port.h"
#include "timer.h"
#include "ad.h"
#include "motor.h"
#include "serial.h"

void system_init(void){
	/* IOポートの初期化 */
	port_init();
	
	/* UARTの初期化 */
	UARTD1_init();
	
	/* ADコンバーターの初期化 */
	AD0_init();
	
	/* タイマーの初期化 */
	TMM0_init();
	TAB1_init();

	/* モーター機能関連の初期化 */
	MOTOR_init();
	
	/* 割込み許可 */
	__asm("ei");
}

