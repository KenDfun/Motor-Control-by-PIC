#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "config_bit.h"

/** LED ************************************************************/
#define mLED0				LATC6
#define mGetLED0()			mLED0
#define mLED0_On()			mLED0 = 1;
#define mLED0_Toggle()     mLED0 = !mLED0;

void init_timer(void);
void init_SPI(void);
/*
 * 
 */
int main(int argc, char** argv)
{
    // Setup Port
	TRISA4 = 0;	// clkout
	TRISC6 = 0; // LED out
	TRISB5 = 1; // Uart RX
	TRISB7 = 0; // Uart TX

	ANSC6 = 0;
	ANSA4 = 0;

    // Setup Clock
    OSCCONbits.IRCF = 0xf; // HFINT 16MHz

    init_timer();
	ei();


	while(1);
	
	return (EXIT_SUCCESS);
}

void init_timer(void)
{
    // timer
	OPTION_REGbits.TMR0CS = 0; // Internal clock
	OPTION_REGbits.PSA = 0; // Use Prescaler
	OPTION_REGbits.PS = 7; // 1:256

	TMR0 = 0;	// clear 8bit counter
    INTCONbits.TMR0IF = 0; // timer0 interrupt flag clear
	INTCONbits.TMR0IE = 1; // enable timer0  interrupt
}

#if 0
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
#endif

void interrupt int_main(void)
{
    static unsigned int count=0;

    if(TMR0IE && TMR0IF){
        TMR0IF = 0;
        count++;

        if(count>=31){
            count = 0;
		mLED0_Toggle();
//            SendFlg=1;
        }
    }
    else{
        while(1);
    }

}


