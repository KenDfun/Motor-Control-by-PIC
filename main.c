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
void init_pwm(void);
void init_SPI(void);
void init_uart(void);

#define PWM_PERIOD_AT_20K 200

int SendFlg=0;
int RcvFlg=0;
char RcvData;
unsigned char PwmDuty=0;
/*
 * 
 */
int main(int argc, char** argv)
{
//	static char SendCh='a';
	
    // Setup Port
	TRISA4 = 0;	// clkout
	TRISC6 = 0; // LED out
	TRISB5 = 1; // Uart RX
	TRISB7 = 0; // Uart TX

	ANSC6 = 0;
	ANSA4 = 0;
	ANSB5 = 0;

    // Setup Clock
    OSCCONbits.IRCF = 0xf; // HFINT 16MHz

    init_timer();
	init_pwm();
	init_uart();
	
	ei();

	while(1){
#if 0 // Test for TX
		if(SendFlg){
			di();
			SendFlg = 0;
			ei();

			TXREG = SendCh++;
			if(SendCh>'z'){
				SendCh='a';
			}
		}
#endif
		if(RcvFlg){
			di();
			RcvFlg=0;
			ei();

			if(RcvData=='a'){
				if(PwmDuty < (PWM_PERIOD_AT_20K/2) ){
					PwmDuty+=10;
					PWM1DCH = PwmDuty;
				}
			}
			else if(RcvData=='b'){
				if(PwmDuty >=10){
					PwmDuty-=10;
					PWM1DCH = PwmDuty;
				}
			}

			TXREG = RcvData;
		}
	}
	
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

void init_pwm(void)
{
	TRISC5 = 1; // disable out
	PWM1CON = 0; // diable PWM1

	PR2 = PWM_PERIOD_AT_20K; // Period 20KHz

	PWM1DCH = 0;
	PWM1DCL = 0<<6;

	PIR1bits.TMR2IF = 0;
	T2CONbits.T2CKPS = 0; // timer2 prescale 1:1
	T2CONbits.TMR2ON = 1; // start timer2

	PWM1CONbits.PWM1EN = 1;
	PWM1CONbits.PWM1OUT = 1;

	while(!PIR1bits.TMR2IF);

	TRISC5 = 0;
	PWM1CONbits.PWM1OE = 1;

	PwmDuty = 0;
	PWM1DCH = PwmDuty;


}

void init_uart(void)
{
	BAUDCONbits.BRG16 = 0;
	TXSTAbits.SYNC = 0;
	TXSTAbits.BRGH = 0;
	SPBRGL = 25;

	RCSTAbits.SPEN = 1;

	TXSTAbits.TXEN = 1;

	PIE1bits.RCIE = 1; // Allow Receive Interrupt
	INTCONbits.PEIE = 1; // allow perepheral interrupt

	RCSTAbits.CREN = 1;
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
			SendFlg=1;
        }
    }
	else if(RCIE && RCIF){
		if(RCSTAbits.FERR){
			while(1);
		}
		else if(RCSTAbits.OERR){
			while(1);
		}
		RcvData = RCREG;
		RcvFlg = 1;
	}
    else{
        while(1);
    }

}


