#include "setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "model.h"
// using namespace std;
#define _XTAL_FREQ 8000000 
#define LED LATAbits.LATA1
int enable = 1;
char str[20];
void Mode1(){   // Todo : Mode1 
    ClearBuffer();
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    LATA = 0;
    while(1){
//        strcpy(str, GetString());
//        if(str[0]=='=')
//        {
//            LATA = 0;
//            break;
//        }
//        else if(str[0]-'0' >= 0 && str[0]-'0' <= 9)
//        {
//            LATA = (str[0]-'0' << 4);
//            ClearBuffer();
//        }
//        else
//        {
//            ClearBuffer();
//        }
        if(RCREG =='=')
        {
            LATA = 0;
            break;
        }
        else if(RCREG -'0' >= 0 && RCREG -'0' <= 9)
        {
            LATA = (RCREG -'0' << 1);
        }
    }
    return ;
}
void Mode2(){   // Todo : Mode2 
    ClearBuffer();
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISBbits.RB0 = 1;
    ADCON1 = 0x0F;
    LATB = 0x00; 
    INT0IF = 0;
    INTCONbits.INT0IE = 1;
    LATA = 0;
    while(1){
        strcpy(str, GetString());
        if(str[0]=='=')
        {
            LATA = 0;
            break;
        }
        else if(str[0]-'0' >= 0 && str[0]-'0' <= 9)
        {
            int n = str[0]-'0' + 1;
            int i = 0;
            enable = 1;
            ClearBuffer();
            while(enable == 1 && GetLen() == 0)
            {
                LATA = (i << 1);
                for(int j=0;j<10;j++)
                {
                     __delay_ms(50);
                    if(enable != 1 || GetLen() != 0)
                    {
                        break;
                    }
                }
                i++;
                i %= n;
            }
        }
    }
    return ;
}
void main(void) 
{
    SYSTEM_Initialize() ;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    int tmp=0;
    float tmp0 = score(20.0, 50.0);
    
    while(1) {
        //UART_Write('a');
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        char data_in = str[0];
        if(data_in=='1')
        {
            ClearBuffer();
            LED = 1;/* turn ON LED */
            //UART_Write_Text(data_in);
            //USART_SendString("LED_ONfuck\n"); /* send LED ON status to terminal */
        }
        else if(data_in=='0')
        {
            ClearBuffer();
            LED = 0;/* turn OFF LED */
            //UART_Write_Text(data_in);
            //USART_SendString("LED_OFF\n");/* send LED ON status to terminal */
        }
        else if(data_in=='t')
        {
            ClearBuffer();
            char response[5]; // ????????????????
            if(tmp==0)
            {
                UART_Write_Text("25*40"); // ????
                tmp=1;
            }
            else
            {
                UART_Write_Text("33*55"); // ????
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