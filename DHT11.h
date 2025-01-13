#ifndef DHT11_H
#define DHT11_H

#include <xc.h>
#include <stdio.h>

#define Data_Out LATC0         /* assign Port pin for data */
#define Data_In PORTCbits.RC0  /* read data from Port pin */
#define Data_Dir TRISCbits.RC0 /* Port direction */
#define _XTAL_FREQ 8000000     /* define _XTAL_FREQ for using internal delay */

/* Function Prototypes */
void DHT11_Start();
void DHT11_CheckResponse();
char DHT11_ReadData();
char *ReadData();
void WriteData(char RH_Decimal, char RH_Integral, char T_Decimal, char T_Integral, char Checksum);

/* External functions to be implemented elsewhere */
void UART_SendString(const char *data);
void Control_Lights(int temperature, int humidity);

#endif /* DHT11_H */
