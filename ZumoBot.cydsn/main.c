
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

int rread(void);

int main()
{
   // printf("\n 1");
    struct sensors_ ref;
    struct sensors_ dig;
    CyGlobalIntEnable; 
    UART_1_Start();
    // printf("\n uart start");
  
    sensor_isr_StartEx(sensor_isr_handler);
    
    reflectance_start();

    motor_start();
    
    IR_led_Write(1);
    // printf("\n next ints");
    

    
            // motor number 1 is LEFT
            // motor number 2 i RIGHT
            int whichmotor = 0;
            //printf("\n motor222");
            // stopping at lines
            int kmk = 0;
     //printf("\n next is for- loop \n");
    for(;;)
    {
        printf("loop start \n");
            
            reflectance_read(&ref);
            //printf("%d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);       //print out each period of reflectance sensors
            reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
            //printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);        //print out 0 or 1 according to results of reflectance period
  
        // Magicnumber for Black/Right
        double infraR = ref.r1 - 3770;
        double infraL = ref.l1 - 3770;
        
        // MAXSPEED /(infraMax 23999 -infraMin 3770)
        double magicNum = 0.012624;

        // Calculate motorspeeds
        double rightMotorRaw = magicNum * infraR;
        double leftMotorRaw = magicNum * infraL;
        
            // Convert speeds to int
            int leftMotorCheck = (int) leftMotorRaw;
            int rightMotorCheck = (int) rightMotorRaw;
            
            // Final speed variables
            int leftMotor;
            int rightMotor;
            
            // criss cross
            leftMotor=rightMotorCheck;
            rightMotor=leftMotorCheck;
            
            
            // MAKE THIS WORK! DRIVES SLOW TO THE FIRST LINE!
            /*int boolstart = 0;
        
            while(boolstart == 0) {
                motor_forward(50,50);
                if (dig.l3 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r3 == 0) {
                    boolstart = 1;
                }
                
            }*/

        // STOPPING AT LINES   
        if (dig.l3 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r3 == 0) {
            //printf("\n KMK ACTIVATED: INT KMK = %i \n", kmk);
            
            if(kmk == 0) {
                motor_stop();
                // Waits for remote
                printf("\n Waiting for remote \n");
                wait_going_down();
                printf("\n Remote recived \n");
                motor_start();
                printf("\n motor start \n");
            } else if (kmk == 2) {
                motor_stop();
                printf("\n Motor stop \n");
            }
            kmk++;
            CyDelay(100);
            motor_turn(leftMotor, rightMotor, 0);
            printf("\n KMK OUT \n"); 
        }
           
        // Hard turn
        
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
        
        
        //MOTORS WONT STOP... LIKE EVER.
        if (leftMotor == rightMotor) {
            leftMotor = 255;
            rightMotor = 255;
        }
        
        // motor speed can not exceed 255
        if (leftMotor > 100) {
            leftMotor = 255;
        }
        
        if (rightMotor <= 1) {
            rightMotor = 1;
        }
        
        if (rightMotor > 100) {
            rightMotor = 255;
        }
        
        if (leftMotor <= 1) {
            leftMotor = 1;
        }
        
        // DRIVE
        motor_turn(leftMotor, rightMotor, 0);
        printf("left: %i Right: %i\n", leftMotor, rightMotor);

        //printf("\n\nLeft motor %d , Right motor %d", leftMotor, rightMotor);
        //printf("\nl1: %d r1: %d\r\n", ref.l1, ref.r1);       //print out each period of reflectance sensors
        //CyDelay(250);
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
