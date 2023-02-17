//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"

#include "code.h"
//=====[Declaration of private defines]========================================

#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define PERIOD 0.02
#define TIME_INCREMENT_MS 2000
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============
PwmOut servo(PF_9);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============
static bool servoMotionState = OFF;

//=====[Declarations (prototypes) of private functions]========================

//=====[Implementations of public functions]===================================
bool servoMotionRead()
{
    return servoMotionState;
}

void servoMotionStateWrite( bool state )
{
    servoMotionState = state;
}


void servoInit(){
    servo.period(PERIOD); // 20ms
    servo.write(DUTY_MIN);
}
void servoMotionOpen(){
        servo.write(DUTY_MAX);
        servoMotionStateWrite(ON);
}

void servoMotionClose(){
        servo.write(DUTY_MIN);
        servoMotionStateWrite(OFF);
}
//=====[Implementations of private functions]==================================
