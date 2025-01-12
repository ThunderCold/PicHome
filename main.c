#include "setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;
#define _XTAL_FREQ 8000000 
#define LED LATAbits.LATA1
int enable = 1;
char str[20];

void main(void) 
{
    SYSTEM_Initialize() ;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    int tmp=0;
    
    while(1) {
        strcpy(str, GetString()); // GetString() in uart.c
        char data_in = str[0];
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
            if(tmp==0)
            {
                UART_Write_Text("25*40"); // PIC18 send data to app (format: tt*hh)
                tmp=1;
            }
            else
            {
                UART_Write_Text("33*55"); // for temp
                tmp=0;
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
    return;
}