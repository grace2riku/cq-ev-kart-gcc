
#define PFC9L	 	(*(volatile unsigned char*)0xFFFFF472)
#define PFCE9L	 	(*(volatile unsigned char*)0xFFFFF712)
#define PMC9L	 	(*(volatile unsigned char*)0xFFFFF452)

#define UD1CTL0 	(*(volatile unsigned char*)0xFFFFFA10)
#define UD1TIC		(*(volatile unsigned char*)0xFFFFF160)
#define UD1RIC		(*(volatile unsigned char*)0xFFFFF15E)
#define UD1SIC		(*(volatile unsigned char*)0xFFFFF15C)
#define UD1CTL1 	(*(volatile unsigned char*)0xFFFFFA11)
#define UD1CTL2 	(*(volatile unsigned char*)0xFFFFFA12)

#define UD1STR 		(*(volatile unsigned char*)0xFFFFFA14)
#define UD1TX 		(*(volatile unsigned char*)0xFFFFFA17)


/* UART1：ログ出力で使用 */
void UARTD1_init(void){
	/* 6bit目 UD1TXE=0：送信動作停止 */
	UD1CTL0 &= 0xBF;
	/* 5bit目 UD1RXE=0：受信動作停止 */
	UD1CTL0 &= 0xDF;
	/* 7bit目 UD1PWR=0：UARTD1動作禁止 */
	UD1CTL0 &= 0x7F;

	/* INTUD1T割込みマスク */
	UD1TIC |= 0x40;
	/* INTUD1T割込みステータスクリア */
	UD1TIC &= 0x7F;
	/* INTUD1R割込みマスク */
	UD1RIC |= 0x40;
	/* INTUD1R割込みステータスクリア */
	UD1RIC &= 0x7F;
	/* INTUD1S割込みマスク */
	UD1SIC |= 0x40;
	/* INTUD1S割込みステータスクリア */
	UD1SIC &= 0x7F;

	/* INTUD1R割込み優先順位：7(最も低い) */
	UD1RIC |= 0x07;
	/* INTUD1S割込み優先順位：7(最も低い) */
	UD1SIC |= 0x07;
	/* INTUD1T割込み優先順位：7(最も低い) */
	UD1TIC |= 0x07;
	
	/* UARTD1 fxp1/32 = 1MHz */
	UD1CTL1 = 0x05;
	/* ，UARTD1のボー・レート（シリアル転送スピード）クロック選択 */
	UD1CTL2 = 0x0d;
	/*  
	* LSBファースト, 8bit, 1ストップビット, パリティビットなし
	*/
	UD1CTL0 = 0x10 | 0x02;
	
	/* UARTD1 TXDD1 P90 */
	PFC9L &= 0xFE;
	PFCE9L |= 0x01;
	PMC9L |= 0x01;	
	/* UARTD1 RXDD1 P91 */
	PFC9L &= 0xFD;
	PFCE9L |= 0x02;
	PMC9L |= 0x02;
}


void UARTD1_Start(void) {
	/* INTUD1T割込みステータスクリア */
	UD1TIC &= 0x7F;
	/* INTUD1T割込み許可 */
	UD1TIC &= 0xBF;
#if 0
	/* INTUD1R割込みステータスクリア */
	UD1RIC &= 0x7F;
	/* INTUD1R割込み許可 */
	UD1RIC &= 0xBF;
	/* INTUD1S割込みステータスクリア */
	UD1SIC &= 0x7F;
	/* INTUD1S割込み許可 */
	UD1SIC &= 0xBF;
#endif
	
	/* 7bit目 UD1PWR=0：UARTD1動作禁止 */
	UD1CTL0 |= 0x80;
	/* 6bit目 UD1TXE=0：送信動作停止 */
	UD1CTL0 |= 0x40;
#if 0
	/* 5bit目 UD1RXE=0：受信動作停止 */
	UD1CTL0 |= 0x20;
#endif
}

static unsigned char* Uartd1TxBufferAddress;
static unsigned short Uartd1TxLen;

int UARTD1_SendData(unsigned char* txbuf, unsigned short len) {
	int status = 0;
	
	if (len < 1) {
		/* 送信データ長異常 */
		status = 1;
	} else {
		Uartd1TxBufferAddress = txbuf;
		Uartd1TxLen = len;
		if ( (UD1STR & 0x80) == 0 ){
			/* INTUD1T割込みマスク */
			UD1TIC |= 0x40;
			/* 送信データセット */
			UD1TX = *Uartd1TxBufferAddress;
			Uartd1TxBufferAddress++;
			Uartd1TxLen--;
			/* INTUD1T割込み許可 */
			UD1TIC &= 0xBF;
			status = 0;
		} else {
			/* 未送信データあり */
			status = 2;
		}
	}
	
	return status;
}

void _interrupt_send_log(void) {
	if (Uartd1TxLen > 0) {
		UD1TX = *Uartd1TxBufferAddress;
		Uartd1TxBufferAddress++;
		Uartd1TxLen--;
	} else {
		;;
	}
}