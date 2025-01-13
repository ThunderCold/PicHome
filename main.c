#include "setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;
#define _XTAL_FREQ 8000000 
#define LED LATAbits.LATA1
int enable = 1;
int mode; // 1:auto, 2:manual
char str[20];

void main(void) 
{
    SYSTEM_Initialize() ;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    int tmp=0;
    mode = 1;
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
        if(mode == 1)
        {
            //UART_Write_Text("a");
            int value = ADC_Read();
            if(value > 100)
            {
                //UART_Write_Text("0");
                LED = 0;
            }
            else
            {
                //UART_Write_Text("1");
                LED = 1;
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