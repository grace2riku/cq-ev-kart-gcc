
#include "port.h"

void port_init(void){
	PMDLH = 0x00;
	
	/*
	* P9H 8bit
	* P9H.0�F����
	* P9H.1�F����
	* P9H.2�F�o�� �z�[���Z���T U�� LED
	* P9H.3�F�o�� �z�[���Z���T V�� LED
	* P9H.4�F�o�� �z�[���Z���T W�� LED
	* P9H.5�F����
	* P9H.6�F����
	* P9H.7�F����
	*/
	PM9H = 0xE3;
}

