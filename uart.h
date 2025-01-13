#ifndef _UART_H
#define _UART_H
      
void UART_Initialize(void);
char * GetString();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void UART_Write_number(int number);
void ClearBuffer();
void MyusartRead();
int GetLen();

#endif