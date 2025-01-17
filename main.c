#include "setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;
#define _XTAL_FREQ 8000000 
#define LED LATAbits.LATA1
#define FAN LATAbits.LATA2
#define LED2 LATAbits.LATA3
#define LED3 LATAbits.LATA4
#define INF_UP PORTDbits.RD0
#define INF_DOWN PORTDbits.RD1
int enable = 1;
int mode = 2; // 1:auto, 2:manual
int cycle = 1; //1:on 2:off
int x;
int cycle_direction = 1; //1:neg 2:pos
int direction = 1; // 1:stop 2:up 3down
char str[20];

#define angle_0 725
#define angle_45 962
#define angle_neg_45 488
#define angle_neg 685
#define angle_pos 795

void main(void) 
{
    SYSTEM_Initialize() ;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISDbits.RD0 = 1;
    TRISDbits.RD1 = 1;
    int tmp=0;
    TRISCbits.TRISC2=0;	// RC2 pin is output.
    //CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = angle_0 >> 2;
    CCP1CONbits.DC1B = angle_0 & 0x03;
    x = angle_0;
    CCPR2L = angle_0 >> 2;
    CCP2CONbits.DC2B = (angle_0 & 0b11);
    while(1) {
        strcpy(str, GetString()); // GetString() in uart.c
        char data_in = str[0];
        char t, h;
        if(data_in=='1')
        {
            ClearBuffer();
            LED = 1;/* turn ON LED */
        }
        else if(data_in=='0')
        {
            ClearBuffer();
            LED = 0;/* turn OFF LED */
        }
        else if(data_in=='t') // request temp. and humi.
        {
            ClearBuffer();
            DHT11_Start();
            DHT11_CheckResponse();
            ReadData(&t, &h);
            char ret[15];
            sprintf(ret, "%02d", t);
            UART_Write_Text(ret);
            /*if(tmp==0)
            {
                UART_Write_Text("25*40"); // PIC18 send data to app (format: tt*hh)
                tmp=1;
            }
            else
            {
                UART_Write_Text("33*55"); // for temp
                tmp=0;
            }*/
        }
          else if(data_in=='h')
        {
            ClearBuffer();
            char ret[15];
            sprintf(ret, "%02d", h);
            UART_Write_Text(ret);
        }
        else if(data_in == 'a')
        {
            //UART_Write_Text("auto!");
            ClearBuffer();
            mode = 1; // auto
        }
        else if(data_in == 'm')
        {
            //UART_Write_Text("manual!");
            ClearBuffer();
            mode = 2; // manual
        }
        else if(data_in=='l') //light
        {
            ClearBuffer();
            UART_Write_number(ADC_Read());
        }
        else if(data_in == 'u')
        {
            ClearBuffer();
            direction = 2;
            CCPR1L = angle_neg >> 2;
            CCP1CONbits.DC1B = angle_neg & 0x03;
        }
        else if(data_in == 'd')
        {
            ClearBuffer();
            direction = 3;
            CCPR1L = angle_pos >> 2;
            CCP1CONbits.DC1B = angle_pos & 0x03;
        }
        else if(data_in == 's')
        {
            ClearBuffer();
            direction = 1;
            CCPR1L = angle_0 >> 2;
            CCP1CONbits.DC1B = angle_0 & 0x03;
        }
        else if(data_in == 'f')
        {
            ClearBuffer();
            FAN = 0; //on
        }
        else if(data_in == 'g')
        {
            ClearBuffer();
            FAN = 1; //off
        }
        else if(data_in == 'y')
        {
            ClearBuffer();
            cycle = 1;
        }
        else if(data_in == 'z')
        {
            ClearBuffer();
            cycle = 2;
        }
        if(mode == 1)
        {
            //UART_Write_Text("a");
            int value = ADC_Read();
            if(value > 100 && INF_UP != 1)
            {
                //UART_Write_Text("0");
                LED = 0;
                direction = 2;
                CCPR1L = angle_neg >> 2;
                CCP1CONbits.DC1B = angle_neg & 0x03;
            }
            else if(value <= 100 && INF_DOWN == 1)
            {
                //UART_Write_Text("1");
                LED = 1;
                direction = 3;
                CCPR1L = angle_pos >> 2;
                CCP1CONbits.DC1B = angle_pos & 0x03;
            }
            if(t >= 25)
            {
                if(direction == 1)
                {
                    cycle = 0;
                    FAN = 0;
                }
            }
            else 
            {
                cycle = 1;
                FAN = 1;
            }
        }
        if(INF_UP == 1)
        {
            if(direction == 2)
            {
                direction = 1;
                CCPR1L = angle_0 >> 2;
                CCP1CONbits.DC1B = angle_0 & 0x03;
            }
            LED2 = 1;
        }
        if(INF_UP == 0)
        {
            LED2 = 0;
        }
        if(INF_DOWN == 1)
        {
            
            LED3 = 1;
        }
        if(INF_DOWN == 0)
        {
            if(direction == 3)
            {
                direction = 1;
                CCPR1L = angle_0 >> 2;
                CCP1CONbits.DC1B = angle_0 & 0x03;
            }
            LED3 = 0;
        }
        

        if(cycle == 1)
        {
            if(cycle_direction == 1)
            {
                x--;
                CCPR2L = x >> 2;
                CCP2CONbits.DC2B = (x & 0b11);
                if(x == angle_neg_45)
                {
                    cycle_direction = 2;
                }
                __delay_ms(5);
            }
            else if(cycle_direction == 2)
            {
                x++;
                CCPR2L = x >> 2;
                CCP2CONbits.DC2B = (x & 0b11);
                if(x == angle_45)
                {
                    cycle_direction = 1;
                }
                __delay_ms(5);
            }
            
        }
        
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{
    //UART_Write_Text("interrupt high\r\n");
    if (INT0IF) {  
        //UART_Write_Text("button push!\r\n");
        enable = 0;  
        ClearBuffer(); 
        INT0IF = 0;    
    }
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
    return;
}