#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
//#include <pic18f45k20.h>


// PIC18F45K20 Configuration Bit Settings

// CONFIG1H
#pragma config FOSC = INTIO7   // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 18        // Brown Out Reset Voltage bits (VBOR set to 1.8 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config DEBUG = OFF
// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

/** LED ************************************************************/
#define mInitAllLEDs()      LATD = 0; TRISD = 0;

#define mLED                LATD
#define mLED_0              LATDbits.LATD0
#define mLED_1              LATDbits.LATD1
#define mLED_2              LATDbits.LATD2
#define mLED_3              LATDbits.LATD3
#define mLED_4              LATDbits.LATD4
#define mLED_5              LATDbits.LATD5
#define mLED_6              LATDbits.LATD6
#define mLED_7              LATDbits.LATD7


#define mGetLED_0()         mLED_0
#define mGetLED_1()         mLED_1
#define mGetLED_2()         mLED_2
#define mGetLED_3()         mLED_3
#define mGetLED_4()         mLED_4
#define mGetLED_5()         mLED_5
#define mGetLED_6()         mLED_6
#define mGetLED_7()         mLED_7


#define mLED_0_On()         mLED_0 = 1;
#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;
#define mLED_5_On()         mLED_5 = 1;
#define mLED_6_On()         mLED_6 = 1;
#define mLED_7_On()         mLED_7 = 1;


#define mLED_0_Toggle()     mLED_0 = !mLED_0;
#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;
#define mLED_4_Toggle()     mLED_4 = !mLED_4;
#define mLED_5_Toggle()     mLED_5 = !mLED_5;
#define mLED_6_Toggle()     mLED_6 = !mLED_6;
#define mLED_7_Toggle()     mLED_7 = !mLED_7;

int SendFlg=0;
void init_timer(void);
void init_SPI(void);
/*
 * 
 */
int main(int argc, char** argv)
{
    volatile unsigned int dummy;
    unsigned char sendData='a';
    
    // Setup Clock
    OSCCONbits.SCS0 = 0;
    OSCCONbits.SCS1 = 0;
    OSCCONbits.IRCF = 7; // HFINTOSC selec
//    while(!OSCCONbits.IOFS);

    OSCTUNEbits.PLLEN = 1; //64MHz

    OSCCONbits.SCS0 = 0;
    OSCCONbits.SCS1 = 0;

    mInitAllLEDs();

    init_timer();
    init_SPI();

    // interrupt enable
    TMR0IE = 1;
    ei();
    
    while(1){
        if(SSPCON1bits.WCOL){
            SSPCON1bits.WCOL = 0;
        }
        if(SendFlg){
            SendFlg=0;
        SSPBUF = (sendData++);
        if(sendData>'z'){
            sendData = 'a';
        }

        while(!SSPSTATbits.BF);
        dummy = SSPBUF;
        }
    }

    return (EXIT_SUCCESS);
}

void init_timer(void)
{
    // timer
    T0CONbits.TMR0ON = 0; // Stops Timer0
    T0CONbits.T08BIT = 1; // 8bit mode
    T0CONbits.T0CS   = 0; // Internal instruction cycle
    T0CONbits.PSA    = 0; // Use Prescaler
    T0CONbits.T0PS   = 7; // 1:256
    TMR0L = 0;

    INTCONbits.T0IF = 0; // timer0 interrupt flag clear
    T0CONbits.TMR0ON = 1; // timer0 start
}

void init_SPI(void)
{
    volatile unsigned int dummy;

    TRISCbits.RC3 = 0; // SCK out
    TRISCbits.RC4 = 1; // SDI in
    TRISCbits.RC5 = 0; // SDO out

    SSPSTATbits.SMP = 0; // SPI Master/Input data sampled at middle of data output time
    SSPSTATbits.CKE = 0; // Output data changes on clock transition from idle to active
//    SSPCON1bits.SSPM = 0; // SPI Master mode, clock = Fosc/4
    SSPCON1bits.SSPM = 1; // SPI Master mode, clock = Fosc/16 4MHz
    SSPCON1bits.CKP = 0; // Idle state for clock is a low level

    SSPCON1bits.SSPEN = 1; // SPI enable

    dummy = SSPBUF; // dummy read from buffer

}

void interrupt int_main(void)
{
    static unsigned int count=0;

    if(TMR0IE && TMR0IF){
        TMR0IF = 0;
        mLED_1_Toggle();
        count++;
//        if(count>122){
        if(count>10){
            count = 0;
            mLED_0_Toggle();
            SendFlg=1;
        }
    }
    else{
        mLED_7_On();
        while(1);
    }

}


