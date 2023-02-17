//=====[#include guards - begin]===============================================

#ifndef _SERVO_H_
#define _SERVO_H_

//=====[Declaration of public defines]=========================================


//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================
bool servoMotionRead();
void servoMotionStateWrite( bool state );
void servoInit();
void servoMotionOpen();
void servoMotionClose();

//=====[#include guards - end]=================================================

#endif // _SERVO_H_
