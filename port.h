
#ifndef _PORT_
#define _PORT_

#define PMDLH	(*(volatile unsigned char*)0xFFFFF025)
#define PDLH 	(*(volatile unsigned char*)0xFFFFF005)

#define PM9H 	(*(volatile unsigned char*)0xFFFFF433)
#define P9L 	(*(volatile unsigned char*)0xFFFFF412)
#define P9H 	(*(volatile unsigned char*)0xFFFFF413)

#define PORT_HALLSENSOR		(P9L)	/* �z�[���Z���T �|�[�g */
#define PORT_LED_HALLSENSOR	(P9H)	/* �z�[���Z���T LED�|�[�g */
#define PORT_LED_FET		(PDLH)	/* FET LED�|�[�g */

#define LED_FET_UHVL_ON		(0x09)	/* UH VL ON */
#define LED_FET_UHWL_ON		(0x21)	/* UH WL ON */
#define LED_FET_VHWL_ON		(0x24)	/* VH WL ON */
#define LED_FET_VHUL_ON		(0x06)	/* VH UL ON */
#define LED_FET_WHUL_ON		(0x12)	/* WH UL ON */
#define LED_FET_WHVL_ON		(0x18)	/* WH VL ON */
#define LED_FET_ALL_ON		(0x3F)	/* �SLED�� */
#define LED_FET_ALL_OFF		(0x00)	/* �SLED���� */

#define HALLSENSOR_U_PORT	(0x04)	/* �z�[���Z���TU�� ���̓|�[�g P9L.2 */
#define HALLSENSOR_V_PORT	(0x08)	/* �z�[���Z���TV�� ���̓|�[�g P9L.3 */
#define HALLSENSOR_W_PORT	(0x10)	/* �z�[���Z���TW�� ���̓|�[�g P9L.4  */

#define LED_HALLSENSOR_U_BIT	(0x04)	/* �z�[���Z���TU�� LED P9H.2*/
#define LED_HALLSENSOR_V_BIT	(0x08)	/* �z�[���Z���TV�� LED P9H.3*/
#define LED_HALLSENSOR_W_BIT	(0x10)	/* �z�[���Z���TW�� LED P9H.4*/

void port_init(void);

#endif