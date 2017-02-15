/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

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

/**
 * @file    main.c
 * @brief   
 * @details  ** You should enable global interrupt for operating properly. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/


//battery level//
/*
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    ADC_Battery_Start();        
    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board

    for(;;)
    {
        
        ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16();
            //volts = ADC_Battery_CountsTo_Volts(adcresult);                  // convert value to Volts
            volts = (((5*(float)adcresult)/4095)*3/2);
            // volts = adcresult*0.0012f*1.5f;
            // If you want to print value
            printf("%d %f\r\n",adcresult, volts);
        }
        CyDelay(500);
        
    }
 }   
//*/


/*//ultra sonic sensor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    while(1) {
        //If you want to print out the value  
        printf("distance = %5.0f\r\n", Ultra_GetDistance());
        CyDelay(1000);
    }
}   
//*/


/*//nunchuk//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    nunchuk_start();
    nunchuk_init();
    
    for(;;)
    {    
        nunchuk_read();
    }
}   
//*/


/*//IR receiver//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    
    unsigned int IR_val; 
    
    for(;;)
    {
       IR_val = get_IR();
       printf("%x\r\n\n",IR_val);
    }    
 }   
//*/


/*//Ambient light sensor//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    
    I2C_Start();
    
    I2C_write(0x29,0x80,0x00);          // set to power down
    I2C_write(0x29,0x80,0x03);          // set to power on
    
    for(;;)
    {    
        uint8 Data0Low,Data0High,Data1Low,Data1High;
        Data0Low = I2C_read(0x29,CH0_L);
        Data0High = I2C_read(0x29,CH0_H);
        Data1Low = I2C_read(0x29,CH1_L);
        Data1High = I2C_read(0x29,CH1_H);
        
        uint8 CH0, CH1;
        CH0 = convert_raw(Data0Low,Data0High);      // combine Data0
        CH1 = convert_raw(Data1Low,Data1High);      // combine Data1

        double Ch0 = CH0;
        double Ch1 = CH1;
        
        double data = 0;
        data = getLux(Ch0,Ch1);
        
        // If you want to print out data
        //printf("%lf\r\n",data);    
    }    
 }   
//*/


/*//accelerometer//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    I2C_Start();
  
    uint8 X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A;
    int16 X_AXIS_A, Y_AXIS_A, Z_AXIS_A;
    
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        //print out accelerometer output
        X_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_A);
        X_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_A);
        X_AXIS_A = convert_raw(X_L_A, X_H_A);
        
        Y_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_A);
        Y_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_A);
        Y_AXIS_A = convert_raw(Y_L_A, Y_H_A);
        
        Z_L_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_A);
        Z_H_A = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_A);
        Z_AXIS_A = convert_raw(Z_L_A, Z_H_A);
        
        printf("ACCEL: %d %d %d %d %d %d \r\n", X_L_A, X_H_A, Y_L_A, Y_H_A, Z_L_A, Z_H_A);
        value_convert_accel(X_AXIS_A, Y_AXIS_A, Z_AXIS_A);
        printf("\n");
        
        CyDelay(50);
    }
}   
//*/


//reflectance//
int main()
{
    printf("***PROGRAM START***");
    struct sensors_ ref;
    struct sensors_ dig;
    CyGlobalIntEnable; 
    UART_1_Start();
  
    sensor_isr_StartEx(sensor_isr_handler);
    
    reflectance_start();

    motor_start();
    
    IR_led_Write(1);
    
    unsigned int IR_val;
    

    
            // motor number 1 is LEFT
            // motor number 2 i RIGHT
            int whichmotor;
    
            // stopping at lines
            int kmkSwag = 0;
    
    for(;;)
    {
            // remote control    
            IR_val = get_IR();
            printf("%x\r\n\n",IR_val);
            CyDelay(500);
            
            reflectance_read(&ref);
            //printf("%d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);       //print out each period of reflectance sensors
            reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
            //printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);        //print out 0 or 1 according to results of reflectance period
            
            
        // Magicnumber for Black/Right
        double infraR = ref.r1 - 3770;
        double infraL = ref.l1 - 3770;
        double magicNum = 0.012624;
            
            // TESTAUKSESSA
            if(infraR < 10000) {
                infraR = 0;
            }
            
            // TESTAUKSESSA
             if(infraL < 10000) {
                infraL = 0;
            }
        
        // Magicnumber for White/Left
        //double magicNumL = ref.r1 - 3880;
        
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
            
            
        // STOPPING AT LINES   
        if (dig.l3 == 1 && dig.l1 && dig.r1 && dig.r3) {
            
            if(kmkSwag == 0) {
                motor_stop();
                // Waits for remote
                wait_going_down();
                motor_start();
            } else if (kmkSwag == 2) {
                motor_stop();
            }
            kmkSwag++;
            
        }
            
        // Hard turn
        
        if (rightMotor < 25 && leftMotor < 25) {
            if (whichmotor == 1) {
                leftMotor = 254;
            } else {
                rightMotor = 254;
            }
        } else if (leftMotor < 20) {
            whichmotor = 2; // right
        } else if (rightMotor < 20) {
            whichmotor = 1; // left
        }
        
        
        // TEST --- MOTORS WONT STOP... LIKE EVER.
        if (leftMotor == rightMotor) {
            leftMotor = 254;
            rightMotor = 254;
        }
        
        // motor speed can not exceed 255
        if (leftMotor >= 255) {
            leftMotor = 254;
        }
        
        if (rightMotor <= 1) {
            rightMotor = 0;
        }
        
        if (rightMotor >= 255) {
            rightMotor = 254;
        }
        
        if (leftMotor <= 1) {
            leftMotor = 0;
        }
        
        // DRIVE
        motor_turn(leftMotor, rightMotor, 0);
        printf("\n\nLeft motor %d , Right motor %d", leftMotor, rightMotor);
        printf("\nl1: %d r1: %d\r\n", ref.l1, ref.r1);       //print out each period of reflectance sensors
        //CyDelay(250);
        /*
  // Check side of the line
        if(ref.l1 > ref.r1) {
            motor_turn(rightMotor, leftMotor, 0);
            printf("\n\nLeft motor %d , Right motor %d", leftMotor, rightMotor);
        } else {
            motor_turn(leftMotor, rightMotor, 0);
            printf("\n\nLeft motor %d , Right motor %d", leftMotor, rightMotor);
        }  

        */
        
        
        
        
        
        /*
                int decelerate;
                int turn = 1;
                int delay = 3;
                int kerroin;
            
            
                if (dig.l1 == 0 && dig.r1 == 0) { //forward
                motor_forward(200 ,2);  
                //decelerate = 100;
                } 
                
                else if (dig.r1 == 1 && dig.l1 == 0) { //left
                //motor_turn(100,200,5);
                decelerate = 200;
                    kerroin = 1;
                    while (turn == 1) {
                        
                        reflectance_read(&ref);
                        //printf("%d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);       //print out each period of reflectance sensors
                        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
                        //printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);        //print out 0 or 1 according to results of reflectance period
                        printf("%i", decelerate);
                        
                        motor_turn(decelerate,200,delay);
                        if (dig.l1 == 0 && dig.r1 == 0) {
                             turn = 0;
                        } else if (decelerate - kerroin < 0){
                            decelerate = 0;
                        } else {
                            decelerate = decelerate - kerroin;
                            kerroin++;
                        }
                    }
                }
                
                
                else if (dig.r1 == 0 && dig.l1 == 1) { //right
                //motor_turn(200,100,5);
                decelerate = 200;
                kerroin = 1;
                while (turn == 1) {
                    
                        reflectance_read(&ref);
                        //printf("%d %d %d %d \r\n", ref.l3, ref.l1, ref.r1, ref.r3);       //print out each period of reflectance sensors
                        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
                        //printf("l3(%d) L1(%d) R1(%d) R3(%d) \r\n", dig.l3, dig.l1, dig.r1, dig.r3);        //print out 0 or 1 according to results of reflectance period
                        printf("%i", decelerate);
                        
                        motor_turn(200,decelerate,delay);
                        if (dig.l1 == 0 && dig.r1 == 0) {
                             turn = 0;
                        } else if (decelerate - kerroin < 0){
                            decelerate = 0;
                        } else {
                            decelerate = decelerate - kerroin;
                            kerroin++;
                        }
                    }
                }*/
                
                /*
                else if (dig.r3 == 1 && dig.l3 == 0) { // right hard
                motor_turn(200,0,5);
                }
                
                else if (dig.l3 == 1 && dig.r3 == 0) { // left hard
                motor_turn(0,200,5);
                }
                */
                
       
        
        
        
    }
}   
//

 /*//motor//

// you go forward fast !
void forward_fast(uint32 time){ // time in milliseconds
    motor_forward(200 ,time);
}

// you turn to left 90 degrees
void left90(){
    motor_turn(0,200,370);
}

// you turn to left 45 degrees
void left45(){
    motor_turn(0,200,185);
}

// you turn to right 90 degrees
void right90(){
    motor_turn(200,0,370);
}

// you turn to right 45 degrees
void right45(){
    motor_turn(200,0,185);
}

int main()
{
    
    CyGlobalIntEnable; 
    UART_1_Start();

    motor_start();              // motor start
   
    forward_fast(1700);
    right90();
    forward_fast(1700);
    right90();
    forward_fast(1400);
    right90();
    right45();
    forward_fast(460);
    motor_turn(150,200,1400);
    //motor_forward(100,2000);     // moving forward
    //motor_turn(200,50,4000);     // turn
    //motor_turn(50,200,4000);     // turn
    //motor_backward(100,2000);    // movinb backward
       
    motor_stop();               // motor stop
    
    for(;;)
    {

    }
} */
//
    

/*//gyroscope//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    I2C_Start();
  
    uint8 X_L_G, X_H_G, Y_L_G, Y_H_G, Z_L_G, Z_H_G;
    int16 X_AXIS_G, Y_AXIS_G, Z_AXIS_G;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps    
    
    for(;;)
    {
        //print out gyroscope output
        X_L_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_L);
        X_H_G = I2C_read(GYRO_ADDR, OUT_X_AXIS_H);
        X_AXIS_G = convert_raw(X_H_G, X_L_G);
        
        
        Y_L_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_L);
        Y_H_G = I2C_read(GYRO_ADDR, OUT_Y_AXIS_H);
        Y_AXIS_G = convert_raw(Y_H_G, Y_L_G);
        
        
        Z_L_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_L);
        Z_H_G = I2C_read(GYRO_ADDR, OUT_Z_AXIS_H);
        Z_AXIS_G = convert_raw(Z_H_G, Z_L_G);
     
        // If you want to print value
        printf("%d %d %d \r\n", X_AXIS_G, Y_AXIS_G, Z_AXIS_G);
        CyDelay(50);
    }
}   
//*/


/*//magnetometer//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
  
    I2C_Start();
   
    uint8 X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M;
    int16 X_AXIS, Y_AXIS, Z_AXIS;
    
    I2C_write(GYRO_ADDR, GYRO_CTRL1_REG, 0x0F);             // set gyroscope into active mode
    I2C_write(GYRO_ADDR, GYRO_CTRL4_REG, 0x30);             // set full scale selection to 2000dps
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL1_REG, 0x37);           // set accelerometer & magnetometer into active mode
    I2C_write(ACCEL_MAG_ADDR, ACCEL_CTRL7_REG, 0x22);
    
    
    for(;;)
    {
        X_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_L_M);
        X_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_X_H_M);
        X_AXIS = convert_raw(X_L_M, X_H_M);
        
        Y_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_L_M);
        Y_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Y_H_M);
        Y_AXIS = convert_raw(Y_L_M, Y_H_M);
        
        Z_L_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_L_M);
        Z_H_M = I2C_read(ACCEL_MAG_ADDR, OUT_Z_H_M);
        Z_AXIS = convert_raw(Z_L_M, Z_H_M);
        
        heading(X_AXIS, Y_AXIS);
        printf("MAGNET: %d %d %d %d %d %d \r\n", X_L_M, X_H_M, Y_L_M, Y_H_M, Z_L_M, Z_H_M);
        printf("%d %d %d \r\n", X_AXIS,Y_AXIS, Z_AXIS);
        CyDelay(50);      
    }
}   
//*/


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
