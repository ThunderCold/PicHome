#include <xc.h>
    //setting TX/RX
#define _XTAL_FREQ 8000000 
#define F_CPU _XTAL_FREQ/64
#define baud_rate 9600
#define Baud_value (((float)(F_CPU)/(float)baud_rate)-1)
char mystring[20];
int lenStr = 0;

void UART_Initialize() {
           
//           TODObasic   
//           Serial Setting      
//        1.   Setting Baud rate
//        2.   choose sync/async mode 
//        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
//        3.5  enable Tx, Rx Interrupt(optional)
//        4.   Enable Tx & RX
           
    //float temp;
    //temp=Baud_value;  
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate
    TXSTAbits.SYNC = 0;  //1 = Synchronous mode 0 = Asynchronous mode          
    BAUDCONbits.BRG16 = 0; //BRG16: 16-Bit Baud Rate Register Enable bit 1 = 16-bit Baud Rate Generator ? SPBRGH and SPBRG, 0 = 8-bit Baud Rate Generator ? SPBRG only (Compatible mode), SPBRGH value ignored         
    TXSTAbits.BRGH = 0; // Asynchronous mode: 1 = High speed  0 = Low speed
    SPBRG = 12;      //4Mhz speed9600
    
   //   Serial enable
    RCSTAbits.SPEN = 1; //SPEN: Serial Port Enable bit 1 = Serial port enabled (configures RX/DT and TX/CK pins as serial port pins), 0 = Serial port disabled (held in Reset)            
    PIR1bits.TXIF = 1; //TXIF: EUSART Transmit Interrupt Flag bit,1 = The EUSART transmit buffer, TXREG, is empty (cleared when TXREG is written), 0 = The EUSART transmit buffer is full
    PIR1bits.RCIF = 0; //RCIF: EUSART Receive Interrupt Flag bit,1 = The EUSART receive buffer, RCREG, is full (cleared when RCREG is read), 0 = The EUSART receive buffer is empty
    TXSTAbits.TXEN = 1; //Transmit Enable bit  1 = Transmit enabled 0 = Transmit disabled         
    RCSTAbits.CREN = 1; //CREN: Continuous Receive Enable 1 = Enables receiver, 0 = Disables receiver       
    PIE1bits.TXIE = 0; //TXIE: EUSART Transmit Interrupt Enable bit, 1 = Enables the EUSART transmit interrupt, 0 = Disables the EUSART transmit interrupt      
    IPR1bits.TXIP = 0; //TXIP: EUSART Transmit Interrupt Priority bit, 1 = High priority, 0 = Low priority         
    PIE1bits.RCIE = 1; //RCIE: EUSART Receive Interrupt Enable bit, 1 = Enables the EUSART receive interrupt, 0 = Disables the EUSART receive interrupt             
    IPR1bits.RCIP = 0; //RCIP: EUSART Receive Interrupt Priority bit, 1 = High priority, 0 = Low priority      
             
    }

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT); //TRMT: Transmit Shift Register Status bit 1 = TSR empty ,0 = TSR full
    TXREG = data;           //write to TXREG will send data 
}


void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 20 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    /* TODObasic: try to use UART_Write to finish this function */
    
    mystring[lenStr] = RCREG;
    /*if(mystring[lenStr] == '\r')
    {
        UART_Write_Text("\r\n");
    }
    else
    {
        UART_Write(mystring[lenStr]);
    }
    
    if(mystring[lenStr] == 'c')
    {
        UART_Write_Text("buffer clear\n");
        ClearBuffer();
    }
    else
    {
        lenStr++;
        mystring[lenStr] = '\0';
        lenStr %= 10;
    }*/
    lenStr++;
    mystring[lenStr] = '\0';
    lenStr %= 10;
    return ;
}

char *GetString(){
    return mystring;
}

int GetLen(){
    return lenStr;
}


// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{
    //UART_Write_Text("interrupt low\r\n");
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
    
   // process other interrupt sources here, if required
    return;
}