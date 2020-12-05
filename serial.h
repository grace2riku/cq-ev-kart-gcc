
#ifndef _SERIAL_
#define _SERIAL_

void UARTD1_init(void);
void UARTD1_Start(void);
int UARTD1_SendData(unsigned char* txbuf, unsigned short len);

#endif