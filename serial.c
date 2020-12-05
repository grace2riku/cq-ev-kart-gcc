
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


/* UART1�F���O�o�͂Ŏg�p */
void UARTD1_init(void){
	/* 6bit�� UD1TXE=0�F���M�����~ */
	UD1CTL0 &= 0xBF;
	/* 5bit�� UD1RXE=0�F��M�����~ */
	UD1CTL0 &= 0xDF;
	/* 7bit�� UD1PWR=0�FUARTD1����֎~ */
	UD1CTL0 &= 0x7F;

	/* INTUD1T�����݃}�X�N */
	UD1TIC |= 0x40;
	/* INTUD1T�����݃X�e�[�^�X�N���A */
	UD1TIC &= 0x7F;
	/* INTUD1R�����݃}�X�N */
	UD1RIC |= 0x40;
	/* INTUD1R�����݃X�e�[�^�X�N���A */
	UD1RIC &= 0x7F;
	/* INTUD1S�����݃}�X�N */
	UD1SIC |= 0x40;
	/* INTUD1S�����݃X�e�[�^�X�N���A */
	UD1SIC &= 0x7F;

	/* INTUD1R�����ݗD�揇�ʁF7(�ł��Ⴂ) */
	UD1RIC |= 0x07;
	/* INTUD1S�����ݗD�揇�ʁF7(�ł��Ⴂ) */
	UD1SIC |= 0x07;
	/* INTUD1T�����ݗD�揇�ʁF7(�ł��Ⴂ) */
	UD1TIC |= 0x07;
	
	/* UARTD1 fxp1/32 = 1MHz */
	UD1CTL1 = 0x05;
	/* �CUARTD1�̃{�[�E���[�g�i�V���A���]���X�s�[�h�j�N���b�N�I�� */
	UD1CTL2 = 0x0d;
	/*  
	* LSB�t�@�[�X�g, 8bit, 1�X�g�b�v�r�b�g, �p���e�B�r�b�g�Ȃ�
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
	/* INTUD1T�����݃X�e�[�^�X�N���A */
	UD1TIC &= 0x7F;
	/* INTUD1T�����݋��� */
	UD1TIC &= 0xBF;
#if 0
	/* INTUD1R�����݃X�e�[�^�X�N���A */
	UD1RIC &= 0x7F;
	/* INTUD1R�����݋��� */
	UD1RIC &= 0xBF;
	/* INTUD1S�����݃X�e�[�^�X�N���A */
	UD1SIC &= 0x7F;
	/* INTUD1S�����݋��� */
	UD1SIC &= 0xBF;
#endif
	
	/* 7bit�� UD1PWR=0�FUARTD1����֎~ */
	UD1CTL0 |= 0x80;
	/* 6bit�� UD1TXE=0�F���M�����~ */
	UD1CTL0 |= 0x40;
#if 0
	/* 5bit�� UD1RXE=0�F��M�����~ */
	UD1CTL0 |= 0x20;
#endif
}

static unsigned char* Uartd1TxBufferAddress;
static unsigned short Uartd1TxLen;

int UARTD1_SendData(unsigned char* txbuf, unsigned short len) {
	int status = 0;
	
	if (len < 1) {
		/* ���M�f�[�^���ُ� */
		status = 1;
	} else {
		Uartd1TxBufferAddress = txbuf;
		Uartd1TxLen = len;
		if ( (UD1STR & 0x80) == 0 ){
			/* INTUD1T�����݃}�X�N */
			UD1TIC |= 0x40;
			/* ���M�f�[�^�Z�b�g */
			UD1TX = *Uartd1TxBufferAddress;
			Uartd1TxBufferAddress++;
			Uartd1TxLen--;
			/* INTUD1T�����݋��� */
			UD1TIC &= 0xBF;
			status = 0;
		} else {
			/* �����M�f�[�^���� */
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