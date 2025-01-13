#include <pic18f4520.h>
#include <xc.h>
#include <stdio.h>

#define Data_Out LATC0         /* assign Port pin for data */
#define Data_In PORTCbits.RC0  /* read data from Port pin */
#define Data_Dir TRISCbits.RC0 /* Port direction */
#define _XTAL_FREQ 8000000     /* define _XTAL_FREQ for using internal delay */

#pragma config OSC = INTIO67 // Oscillator Selection bits
#pragma config WDT = OFF     // Watchdog Timer Enable bit
#pragma config PWRT = OFF    // Power-up Enable bit
//#pragma config BOREN = ON    // Brown-out Reset Enable bit
//#pragma config PBADEN = OFF  // PORTB A/D Enable bit
#pragma config LVP = OFF     // Low Voltage Programming Enable bit
#pragma config CPD = OFF     // Data EEPROM Code Protection bit

void DHT11_Start();
void DHT11_CheckResponse();
char DHT11_ReadData();
void ReadData(char *t, char *h);
void WriteData(char RH_Decimal, char RH_Integral, char T_Decimal, char T_Integral, char Checksum);

void ReadData(char *t, char *h){
    char RH_Decimal;
    char RH_Integral;
    char T_Decimal;
    char T_Integral;
    char Checksum;
    RH_Integral = DHT11_ReadData();  /* Read RH integral value */
    RH_Decimal = DHT11_ReadData();   /* Read RH decimal value */
    T_Integral = DHT11_ReadData();   /* Read Temp integral value */
    T_Decimal = DHT11_ReadData();    /* Read Temp decimal value */
    Checksum = DHT11_ReadData();     /* Read checksum */
    *t = T_Integral;
    *h = RH_Integral;
    /*char ret[20];
    sprintf(ret, "%d*%d", T_Integral, RH_Integral);
    return ret;*/
}

void WriteData(char RH_Decimal, char RH_Integral, char T_Decimal, char T_Integral, char Checksum){

    int humidity,temperature;
    char buffer[50];
    /* Validate checksum */
    if (Checksum == (RH_Integral + RH_Decimal + T_Integral + T_Decimal))
    {
        humidity = RH_Integral;
        temperature = T_Integral;

        /* Format and send data via UART */
        sprintf(buffer, "Temperature: %d.%dC, Humidity: %d.%d%%\r\n", T_Integral, T_Decimal, RH_Integral, RH_Decimal);
        UART_SendString(buffer);

        /* Control lights based on temperature and humidity */
        Control_Lights(temperature, humidity);
    }
    else
    {
        UART_SendString("Error: Invalid data received from DHT11.\r\n");
        PORTA = 0x00; /* Turn off all lights in case of error */
    }

}

char DHT11_ReadData()
{
    char i, data = 0;  
    for (i = 0; i < 8; i++)
    {
        while (!(Data_In & 1));  /* Wait for start of data pulse */
        __delay_us(30);         
        if (Data_In & 1)        /* Check if data bit is 1 */    
            data = ((data << 1) | 1); 
        else
            data = (data << 1);  /* Data bit is 0 */
        while (Data_In & 1);    /* Wait for end of data pulse */
    }
    return data;
}

void DHT11_Start()
{    
    Data_Dir = 0;  /* Set as output port */
    Data_Out = 0;  /* Send low pulse of min. 18 ms width */
    __delay_ms(18);
    Data_Out = 1;  /* Pull data bus high */
    __delay_us(20);
    Data_Dir = 1;  /* Set as input port */    
}

void DHT11_CheckResponse()
{
    while (Data_In & 1);  /* Wait till bus is low */     
    while (!(Data_In & 1));  /* Wait till bus is high */
    while (Data_In & 1);  /* Wait till bus is low again */
}