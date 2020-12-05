#define OSTS 	(*(volatile unsigned char*)0xFFFFF6C0)
#define PRCMD	(*(volatile unsigned char*)0xFFFFF1FC)
#define PCC		(*(volatile unsigned char*)0xFFFFF828)
#define OSTC	(*(volatile unsigned char*)0xFFFFF6C2)
#define MCM		(*(volatile unsigned char*)0xFFFFF860)
#define PLLS	(*(volatile unsigned char*)0xFFFFF6C1)
#define PLLCTL	(*(volatile unsigned char*)0xFFFFF82C)
#define LOCKR	(*(volatile unsigned char*)0xFFFFF824)
#define RCM		(*(volatile unsigned char*)0xFFFFF80C)
#define SELCNT4	(*(volatile unsigned char*)0xFFFFF3F8)
#define PRSM0	(*(volatile unsigned char*)0xFFFFF8B0)
#define PSC		(*(volatile unsigned char*)0xFFFFF1FE)
#define WDTM2	(*(volatile unsigned char*)0xFFFFF6D0)


void clock_init(void){
	unsigned char prccmd_reg_write_value = 0;
	
	OSTS = 0x06;	/* ���U���莞��2^16/fx */
	
	/* 
	* PCC���W�X�^�ݒ�l = 0x80
	* �T�u�N���b�N�����A�Ғ�R�F�g�p���Ȃ�
	* ���C���N���b�N���U��H�F���U����
	* ���C���E�N���b�N���U��H�̓����A�Ғ�R�F�g�p����
	* CPU�N���b�N�ifcpu�j�̏�ԁF���C���E�N���b�N�ifxx�j����
	* �N���b�N�Ffxx �����Ȃ�
	*/
	prccmd_reg_write_value = 0x80;
	PRCMD = prccmd_reg_write_value;
	PCC = prccmd_reg_write_value;
	
	/* ���C���E�N���b�N���U��H���U���芮���܂ő҂� */
	while (!OSTC) {
		;;
	}
	
	/* PLL���b�N�A�b�v���ԁF2^13/fx�i���W�X�^�����l�Ɠ��������ǈꉞ�ݒ�j */
	PLLS = 0x03;
	/* PLL��������ɂ��� */ 
	PLLCTL |= 0x01;
	/* PLL����҂� */
	while(LOCKR) {
		;;
	}
	
	/*
	* MCM���W�X�^�ݒ�l = 0x01
	* ���C���E�N���b�N�ifxx�j�̑I���F���C���E�N���b�N���U�N���b�N�ifx�j
	*/
	prccmd_reg_write_value = 0x01;
	PRCMD = prccmd_reg_write_value;
	MCM = prccmd_reg_write_value;
	/* CPU����N���b�N��PLL���[�h�ɂ��� */ 
	PLLCTL |= 0x02;
	/* CPU�N���b�N(fcpu)�̐ݒ� */
	prccmd_reg_write_value = PCC | 0x00;
	PRCMD = prccmd_reg_write_value;
	PCC = prccmd_reg_write_value;
	/*
	* �����������U�퓮��̋���
	* �ᑬ�������U��̋���
	*/
	RCM = 0x80;
	/* ���ӃN���b�N�ifxp1�j= fxx */
	SELCNT4 = 0x00;
	/* 
	* ���v�^�C�}�ƃN���b�N�������V���A���E�C���^�t�F�[�XCSIB0�ɋ��������N���b�NfBRG�𐶐�����v���X�P�[��3�𐧌�
	* �v���X�P�[��3�o�́F�֎~
	*/ 
	PRSM0 = 0x00;
	/*
	* PSC���W�X�^�ݒ�l = 0x00
	* INTWDT2�M���ɂ��X�^���o�C�E���[�h����������
	* NMI�[�q���͂ɂ��X�^���o�C�E���[�h����������
	* �}�X�J�u�����荞�ݗv���M���ɂ��X�^���o�C�E���[�h����������
	* �X�^���o�C�E���[�h�̐ݒ�F�ʏ탂�[�h
	*/
	prccmd_reg_write_value = 0x00;
	PRCMD = prccmd_reg_write_value;
	PSC = prccmd_reg_write_value;
	/* �E�I�b�`�h�b�N�^�C�}2�̒�~ */
	WDTM2 = 0x1F;
}
