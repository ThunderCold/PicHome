#include <xc.h>

void ADC_Initialize(void) {
    TRISAbits.RA0 = 1;	// Set as input port
	// Set as input port
    //ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin AN0
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; //AN0 is analog input,other are digital
    // ADCON2 = 0x92;  	
    ADCON2bits.ADFM = 0;    //left justified 
    ADCON2bits.ADCS = 0b000;  //chaek table and put 000(1Mhz < 2.86Mhz) into ADCS
    ADCON2bits.ACQT = 0b001;  //Tad = 2 us acquisition time is Tad = 4 > 2.4
    
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
    
    //ADCON1bits.VCFG0 = 0;
    //ADCON1bits.VCFG1 = 0;
    //ADCON1bits.PCFG = 0b1110; //AN0 is analog input,other are digital
    
    
    
}

int ADC_Read()
{
    int digital;
    
    ADCON0bits.CHS =  0b0000; // Select Channe7
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    //digital = (ADRESH*256) | (ADRESL); // Right justified
    digital = (ADRESH << 2) + (ADRESL >> 6);
    return(digital);
}