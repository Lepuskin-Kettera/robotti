
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
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
    srand(time(NULL));
    struct sensors_ ref;
    struct sensors_ dig;
    CyGlobalIntEnable; 
    UART_1_Start();
  
    printf("main start\n");
    
    sensor_isr_StartEx(sensor_isr_handler);
    
    reflectance_start();

    IR_led_Write(1);
    
    int findEdgeBool = 1;
    int goBack = 0;
    
    motor_start();
    
    int prepare = 1;
    
    // Loop drives to the edge of the arena and waits for the remote command
    while (prepare == 1) {
        
        reflectance_read(&ref);
        reflectance_digital(&dig);
        
//------Debug start-------------------------------------------------------------------------
        
        // DO NOT REMOVE THESE. DOESN'T WORK WITHOUT THEM! I DONT KNOW WHY!
        
        int loopCounter = 1;
        printf("loop: %i \n", loopCounter);
        loopCounter++;
        printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
//------Debug end---------------------------------------------------------------------------
        

        if (dig.l3 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r3 == 0) {
        
            printf("Waiting for command\n");
            motor_turn(0, 0, 0);
            wait_going_down();
            prepare = 0;
        } else {
            motor_forward(50,0);
        }
    }
    
    // BATTLE BEGINS NOW!!
    
    // Preparations for the firts blow
    motor_drive(255,100,700);
    motor_drive(-255,255,250);
    
    // Battle mode loop
    for(;;)
    {
     
        reflectance_read(&ref);
        reflectance_digital(&dig);
        printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
        
        
        // Loop drives forward until robot finds the edge
        while(findEdgeBool == 1) {
            
            reflectance_read(&ref);
            reflectance_digital(&dig);
            printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
            
            
            // As long its white drive forward
            if(dig.l3 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r3 == 1) {    
                motor_forward(255,0);
                printf("motor forward\n");
                
            // But if you see black...    
            } else {
            
                motor_backward(255,250);
                
            // ... Enter this loop, which backs away from the line
            while(goBack < 400) {
                motor_backward(255,1);
                printf("motor backward\n");
                
                // Fail safe. If you see black while going backwards, something is wrong. Now drive forward and get out of the loop
                if(dig.l3 == 0 || dig.l1 == 0 || dig.r1 == 0 || dig.r3 == 0) {
                    motor_forward(255,goBack);
                    goBack = 400;
                }
                
                goBack++;
            }
            
            findEdgeBool = 0;
            motor_backward(0,0);
        }
    }
        
        
        // Lets reset this counter if we need it later
        goBack = 0;
        
        // If (and when) the first blow didn't work out as planed, enter this defencive battle loop
        for (;;) {
            
            
            
            findEdgeBool = 1;
            
            reflectance_read(&ref);
            reflectance_digital(&dig);
            printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
            
            
            // Main idea is to spin fast at the middle of the arena and to wait for the command to attack
            motor_drive(-255,255,5);
            //motor_drive(255,-255,1000);
            
            int attackCommand;
            attackCommand = rand() % 100;
            printf("%i\n", attackCommand);
            
            if(attackCommand == 1){
                printf("ATTACK COMMAND!\n");
                attackCommand = 1;
            }
            
            // Oh, there is the command! STRIKE!
            if(attackCommand == 1) {
                while(findEdgeBool == 1) {
            
                    reflectance_read(&ref);
                    reflectance_digital(&dig);
                    //printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
                    
                    // BASH FORWARD UNTIL YOU SEE BLACK!!!
                    if(dig.l3 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r3 == 1) {    
                    motor_forward(255,0);
                    printf("motor forward\n");
                    
                    // Oh crap, we are at the edge. Fortunately, we have a strategy for this one in mind
                    } else {
                        
                        motor_backward(255,250);
                        
                        while(goBack < 650) {
                            motor_backward(255,1);
                            printf("motor backward\n");
                
                            // Fail safe. If you see black while going backwards, something is wrong. Now drive forward and get out of the loop
                            if(dig.l3 == 0 || dig.l1 == 0 || dig.r1 == 0 || dig.r3 == 0) {
                                motor_forward(255,goBack);
                                goBack = 650;
                            }
                            
                            goBack++;
                        }
                        findEdgeBool = 0;
                    }
                }
                attackCommand = 0;
            }
        }
     
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
