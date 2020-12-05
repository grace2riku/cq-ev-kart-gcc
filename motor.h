
#ifndef _MOTOR_
#define _MOTOR_

void MOTOR_init(void);
void MOTOR_Start(void);

void updateTargetDutyCalcTiming(void);
void clearTargetDutyCalcTiming(void);
int isTargetDutyCalcTiming(void);

void calcDriveDuty(const unsigned short accel_value, unsigned short* driveDuty);
void setDriveDuty(const unsigned short accel_value, const unsigned short driveDuty);

#endif