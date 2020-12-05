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
	
	OSTS = 0x06;	/* 発振安定時間2^16/fx */
	
	/* 
	* PCCレジスタ設定値 = 0x80
	* サブクロック内部帰還抵抗：使用しない
	* メインクロック発振回路：発振許可
	* メイン・クロック発振回路の内蔵帰還抵抗：使用する
	* CPUクロック（fcpu）の状態：メイン・クロック（fxx）動作
	* クロック：fxx 分周なし
	*/
	prccmd_reg_write_value = 0x80;
	PRCMD = prccmd_reg_write_value;
	PCC = prccmd_reg_write_value;
	
	/* メイン・クロック発振回路発振安定完了まで待つ */
	while (!OSTC) {
		;;
	}
	
	/* PLLロックアップ時間：2^13/fx（レジスタ初期値と同じだけど一応設定） */
	PLLS = 0x03;
	/* PLL動作を許可にする */ 
	PLLCTL |= 0x01;
	/* PLL安定待ち */
	while(LOCKR) {
		;;
	}
	
	/*
	* MCMレジスタ設定値 = 0x01
	* メイン・クロック（fxx）の選択：メイン・クロック発振クロック（fx）
	*/
	prccmd_reg_write_value = 0x01;
	PRCMD = prccmd_reg_write_value;
	MCM = prccmd_reg_write_value;
	/* CPU動作クロックをPLLモードにする */ 
	PLLCTL |= 0x02;
	/* CPUクロック(fcpu)の設定 */
	prccmd_reg_write_value = PCC | 0x00;
	PRCMD = prccmd_reg_write_value;
	PCC = prccmd_reg_write_value;
	/*
	* 高速内蔵発振器動作の許可
	* 低速内蔵発振器の許可
	*/
	RCM = 0x80;
	/* 周辺クロック（fxp1）= fxx */
	SELCNT4 = 0x00;
	/* 
	* 時計タイマとクロック同期式シリアル・インタフェースCSIB0に供給されるクロックfBRGを生成するプリスケーラ3を制御
	* プリスケーラ3出力：禁止
	*/ 
	PRSM0 = 0x00;
	/*
	* PSCレジスタ設定値 = 0x00
	* INTWDT2信号によるスタンバイ・モード解除を許可
	* NMI端子入力によるスタンバイ・モード解除を許可
	* マスカブル割り込み要求信号によるスタンバイ・モード解除を許可
	* スタンバイ・モードの設定：通常モード
	*/
	prccmd_reg_write_value = 0x00;
	PRCMD = prccmd_reg_write_value;
	PSC = prccmd_reg_write_value;
	/* ウオッチドックタイマ2の停止 */
	WDTM2 = 0x1F;
}
