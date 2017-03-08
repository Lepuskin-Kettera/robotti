
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
    struct sensors_ ref;
    struct sensors_ dig;
    CyGlobalIntEnable; 
    UART_1_Start();
  
    printf("main start\n");
    
    sensor_isr_StartEx(sensor_isr_handler);
    
    reflectance_start();

    IR_led_Write(1);
    printf("SAATANA\n");

    printf("motor_start\n");
    
    int findEdgeBool = 1;
    wait_going_down();
    motor_start();
    int alku = 1;
    while (alku == 1) {
        
        motor_forward(50,0);
        reflectance_read(&ref);
        reflectance_digital(&dig);
    
    if (dig.l3 == 0 && dig.l1 == 0 && dig.r1 == 0 && dig.r3 == 0) {
    
        motor_stop();
        wait_going_down();
        motor_start();
        alku = 0;
    }
    }
    motor_drive(255,100,700);
    motor_drive(-255,255,250);
    
    for(;;)
    {
     
        reflectance_read(&ref);
        reflectance_digital(&dig);
        printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
        
        while(findEdgeBool == 1) {
            
             reflectance_read(&ref);
        reflectance_digital(&dig);
        printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);
            
            
        if(dig.l3 == 1 && dig.l1 == 1 && dig.r1 == 1 && dig.r3 == 1) {    
        motor_forward(255,0);
        printf("motor forward\n");
        } else {
            motor_backward(255,650);
             printf("motor backward\n");
            findEdgeBool = 0;
            motor_backward(0,0);
        }
    }
        
        for (;;) {
            motor_drive(-255,255,1000);
            motor_drive(255,-255,1000);
            printf("defece\n");
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
