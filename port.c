
#include "port.h"

void port_init(void){
	PMDLH = 0x00;
	
	/*
	* P9H 8bit
	* P9H.0：入力
	* P9H.1：入力
	* P9H.2：出力 ホールセンサ U相 LED
	* P9H.3：出力 ホールセンサ V相 LED
	* P9H.4：出力 ホールセンサ W相 LED
	* P9H.5：入力
	* P9H.6：入力
	* P9H.7：入力
	*/
	PM9H = 0xE3;
}

