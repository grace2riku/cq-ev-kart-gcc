
#ifndef _TIMER_
#define _TIMER_


void TMM0_init(void);
void TMM0_start(void);
void TMM0_stop(void);
void clear_motor_control_cyclic_flag();
unsigned char get_motor_control_cyclic_flag(void);

void TAB1_init(void);
void TAB1_start(void);
void TAB1_stop(void);

	
#endif