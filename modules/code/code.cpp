//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "code.h"

#include "user_interface.h"
#include "pc_serial_com.h"
#include "date_and_time.h"
#include "temperature_sensor.h"
#include "gas_sensor.h"
#include "matrix_keypad.h"
#include "servo.h"
#include "user_interface.h"
#include "display.h"

//=====[Declaration of private defines]========================================
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

extern char codeSequenceFromUserInterface[CODE_NUMBER_OF_KEYS];
extern char codeSequenceFromPcSerialCom[CODE_NUMBER_OF_KEYS];

//=====[Declaration and initialization of private global variables]============

static int numberOfIncorrectCodes = 0;
static char codeSequence[CODE_NUMBER_OF_KEYS] = { '1', '8', '0', '5' };
static char GatecodeSequence[CODE_NUMBER_OF_KEYS] = { '1', '1', '2', '1' };

//=====[Declarations (prototypes) of private functions]========================

static bool codeMatch( char* codeToCompare );
static bool GatecodeMatch( char* codeToCompare );
static void codeDeactivate();

//=====[Implementations of public functions]===================================

void codeWrite( char* newCodeSequence )
{
    int i;
    for (i = 0; i < CODE_NUMBER_OF_KEYS; i++) {
        codeSequence[i] = newCodeSequence[i];
    }
}

bool codeMatchFrom( codeOrigin_t codeOrigin )
{
    bool codeIsCorrect = false;
    bool GatecodeIsCorrect = false;
    static int Remaintime = 3;
    char time[2] = " ";

    switch (codeOrigin) {
        case CODE_KEYPAD:
            if( userInterfaceCodeCompleteRead() ) {
                GatecodeIsCorrect = GatecodeMatch(codeSequenceFromUserInterface);
                userInterfaceCodeCompleteWrite(false);
                if ( GatecodeIsCorrect ) {
                    displayCharPositionWrite(0,0);
                    displayStringWrite("Code correct");
                    servoMotionOpen();
                    delay(2000);
                    Remaintime = 3;
                    userInterfaceInit();

                } else {
                    displayCharPositionWrite(0,0);
                    displayStringWrite("Code incorrect");
                    incorrectCodeStateWrite(ON);
                    numberOfIncorrectCodes++;
                    delay(2000);
                    clearScreen();
                    displayCharPositionWrite(0,0);
                    displayStringWrite("Try again");
                    displayCharPositionWrite(0,1);
                    displayStringWrite("Remain time:");
                    displayCharPositionWrite(13,1);
                    sprintf(time,"%d",Remaintime);
                    displayStringWrite(time);
                    Remaintime = Remaintime - 1;
                }
            }


        break;
        case CODE_PC_SERIAL:
            if( pcSerialComCodeCompleteRead() ) {
                codeIsCorrect = codeMatch(codeSequenceFromPcSerialCom);
                pcSerialComCodeCompleteWrite(false);
                if ( codeIsCorrect ) {
                    codeDeactivate();
                    pcSerialComStringWrite( "\r\nThe code is correct\r\n\r\n" );
                } else {
                    incorrectCodeStateWrite(ON);
                    numberOfIncorrectCodes++;
                    pcSerialComStringWrite( "\r\nThe code is incorrect\r\n\r\n" );
                }
            }

        break;
        default:
        break;
    }

    if ( numberOfIncorrectCodes >= 4 ) {
        systemBlockedStateWrite(ON);
        clearScreen();
        displayCharPositionWrite(0,0);
        displayStringWrite("System");
        displayCharPositionWrite(0,1);
        displayStringWrite("Blocked");
        }


    return codeIsCorrect;
    return GatecodeIsCorrect;
}

//=====[Implementations of private functions]==================================

static bool codeMatch( char* codeToCompare )
{
    int i;
    for (i = 0; i < CODE_NUMBER_OF_KEYS; i++) {
        if ( codeSequence[i] != codeToCompare[i] ) {
            return false;
        }
    }
    return true;
}
static bool GatecodeMatch( char* codeToCompare )
{
    int i;
    for (i = 0; i < CODE_NUMBER_OF_KEYS; i++) {
        if ( GatecodeSequence[i] != codeToCompare[i] ) {
            return false;
        }
    }
    return true;
}
static void codeDeactivate()
{
    systemBlockedStateWrite(OFF);
    incorrectCodeStateWrite(OFF);
    numberOfIncorrectCodes = 0;
}
