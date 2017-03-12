
#include <project.h>
#include <stdio.h>
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "IR.h"
#include "Ambient.h"
#include "Beep.h"
#include "math.h"


#define MAGICNUM 0.012624 /*
MAGICNUM is calculated with a following logic:
MAXSPEED /(infraMax 23999 -infraMin 3770)
*/

int rread(void);

int main()
{

    
//----[STARTING ESSENTIAL COMPONENTS. DO NOT ALTER THESE]----------
    struct sensors_ ref;
    struct sensors_ dig;
    CyGlobalIntEnable; 
    UART_1_Start();
    sensor_isr_StartEx(sensor_isr_handler);
    reflectance_start();
    motor_start();
    IR_led_Write(1);
//-----------------------------------------------------------------
    
    int whichmotor = 0; /*
    Program uses this to find back to the track if the robot looses it.
    motor number 1 is LEFT
    motor number 2 is RIGHT
    */

    int kmk = 0; /*
    Counter for start/stop funtion at the start and the finishline
    */
    
    
    for(;;)
    {
        
//------[1. CALCULATE MOTORSPEEDS]------------------------------------------------------------------------------------------
        
        reflectance_read(&ref);
        reflectance_digital(&dig);

        double infraR = ref.r1 - 3770;
        double infraL = ref.l1 - 3770;
        double rightMotorRaw = MAGICNUM * infraL;
        double leftMotorRaw = MAGICNUM * infraR;
        
        // Convert speeds to int
        int leftMotor = (int) leftMotorRaw;
        int rightMotor = (int) rightMotorRaw;
            
//------[2. STOPPING AT LINES]----------------------------------------------------------------------------------------------

        // Used for the start and the end of the race.

        if (dig.l3 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r3 == 0) {

            if(kmk == 0) /* First black line = START */ {
                motor_stop();
                wait_going_down(); // Waits for the command from the remote control
                motor_start();
            } else if (kmk == 2) /* Third black line = GOAL */ {
                motor_stop();
            }
            
            // Do these at every line
            kmk++;
            CyDelay(100);
            motor_turn(leftMotor, rightMotor, 0);
            
        }

//------[3. HARD TURN]------------------------------------------------------------------------------------------------------      
/*
        If the robot drives away from the track,
        this figures out that from which side the robot has driven out and then drives back.
*/    
        if (rightMotor < 80 && leftMotor < 80) {
            if (whichmotor == 1) {
                leftMotor = 255;
            } else {
                rightMotor = 255;
            }
        } else if (leftMotor < 80) {
            whichmotor = 2; // right
        } else if (rightMotor < 80) {
            whichmotor = 1; // left
        }
        
//------[4. ASSISTIVE IF STATEMENTS]----------------------------------------------------------------------------------------      
        
        // If motors get a same speed, go full speed. This makes the robot faster and makes the accelerations faster
        if (leftMotor == rightMotor) {
            leftMotor = 255;
            rightMotor = 255;
        }
        
        // Motor speed can't exceed 255. Ensures the stability of the motor
        if (leftMotor > 100) {
            leftMotor = 255;
        }
        
        if (rightMotor > 100) {
            rightMotor = 255;
        }
        
        
        // Motor speed can't go lower than 1. Ensures the stability of the motor
        if (rightMotor <= 1) {
            rightMotor = 1;
        }
        
        if (leftMotor <= 1) {
            leftMotor = 1;
        }
        
//------[5. DRIVE]--------------------------------------------------------------------------------------------------------
        
        motor_turn(leftMotor, rightMotor, 0);

    }
}   






#if 0
int rread(void)
{
    SC0_SetDriveMode(PIN_DM_STRONG);
    SC0_Write(1);
    CyDelayUs(10);
    SC0_SetDriveMode(PIN_DM_DIG_HIZ);
    Timer_1_Start();
    uint16_t start = Timer_1_ReadCounter();
    uint16_t end = 0;
    while(!(Timer_1_ReadStatusRegister() & Timer_1_STATUS_TC)) {
        if(SC0_Read() == 0 && end == 0) {
            end = Timer_1_ReadCounter();
        }
    }
    Timer_1_Stop();
    
    return (start - end);
}
#endif

/* Don't remove the functions below */
int _write(int file, char *ptr, int len)
{
    (void)file; /* Parameter is not used, suppress unused argument warning */
	int n;
	for(n = 0; n < len; n++) {
        if(*ptr == '\n') UART_1_PutChar('\r');
		UART_1_PutChar(*ptr++);
	}
	return len;
}

int _read (int file, char *ptr, int count)
{
    int chs = 0;
    char ch;
 
    (void)file; /* Parameter is not used, suppress unused argument warning */
    while(count > 0) {
        ch = UART_1_GetChar();
        if(ch != 0) {
            UART_1_PutChar(ch);
            chs++;
            if(ch == '\r') {
                ch = '\n';
                UART_1_PutChar(ch);
            }
            *ptr++ = ch;
            count--;
            if(ch == '\n') break;
        }
    }
    return chs;
}
/* [] END OF FILE */
