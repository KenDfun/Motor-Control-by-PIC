#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include "config_bit.h"

/** LED ************************************************************/
#define mLED0			LATC6
#define mGetLED0()		mLED0
#define mLED0_On()		mLED0 = 1;
#define mLED0_Toggle()          mLED0 = !mLED0;

#define mLED1			LATC4
#define mGetLED1()		mLED1
#define mLED1_On()		mLED1 = 1;
#define mLED1_Off()		mLED1 = 0;
#define mLED1_Toggle()          mLED1 = !mLED1;

void init_timer(void);
void init_pwm(void);
void init_SPI(void);
void init_uart(void);

void sendUartStr(char *str);
void sendUartCh(char ch);

void procCommand(int num);

#define PWM_PERIOD_AT_20K 200

int SendFlg=0;

char RcvData;
unsigned char PwmDuty=0;

char tmpBuf[15];

typedef enum {
    STATE_RCV_INIT = 0,
    STATE_RCV_WAIT_COMMAND,
    STATE_RCV_GET_COMMAND
} STATE_RCV;

STATE_RCV StateRcv = STATE_RCV_INIT;
int RcvStartFlg = 0;
int RcvFlg = 0;
int RcvCount;

/*
 * 
 */
int main(int argc, char** argv)
{
//	static char SendCh='a';
    int num;


    // Setup Port
	TRISA4 = 0;	// clkout
	TRISC6 = 0; // LED out
        TRISC4 = 0; // LED out
	TRISB5 = 1; // Uart RX
	TRISB7 = 0; // Uart TX
        TRISC5 = 0; //PWM1 out
        TRISC3 = 0; //PWM2 out

	ANSC6 = 0;
	ANSA4 = 0;
	ANSB5 = 0;
        ANSC3 = 0;
    //    LATC5=1;
    // Setup Clock
    OSCCONbits.IRCF = 0xf; // HFINT 16MHz

    init_timer();
	init_pwm();
	init_uart();
        clearBuf();

        sendUartStr((char *)"Start up!\r\n");

        ei();

        RcvCount = 0;
        StateRcv=STATE_RCV_WAIT_COMMAND;
        clearBuf();


	while(1){


            if(RcvFlg){
                di();
                RcvFlg=0;
                ei();

                switch(StateRcv){
                case STATE_RCV_WAIT_COMMAND:
                    if(RcvData=='('){
                        StateRcv=STATE_RCV_GET_COMMAND;
                    }
                    break;

                case STATE_RCV_GET_COMMAND:
                    if(RcvData==')'){
                        StateRcv=STATE_RCV_WAIT_COMMAND;
                        RcvCount = 0;

                        num=chkBuf();
                        if(num!=-1){
                            procCommand(num);

                        }
                        clearBuf();
                    }
                    else{
                        putBuf(RcvData);
                    }
                    break;

                default:
                    break;

                }

//                sendUartCh(RcvData);
        }
    }
	
	return (EXIT_SUCCESS);
}

void procCommand(int num)
{
    int result=0;
                sprintf(tmpBuf,"%d:\r\n",num);
                sendUartStr(tmpBuf);

    switch(num){
        case CMD_NUM_ON_LED1:
            mLED1_On();
            result = 1;
            break;

        case CMD_NUM_OFF_LED1:
            mLED1_Off();
            result = 1;
            break;

        case CMD_NUM_UP_PWM1:
            if(PwmDuty<200){
                PwmDuty += 0x8;
            }
            PWM1DCH = PwmDuty;
            result = 1;
            break;

        case CMD_NUM_DOWN_PWM1:
            if(PwmDuty>=0x8){
                PwmDuty -= 0x8;
            }
            PWM1DCH = PwmDuty;
            result = 1;
            break;

        default:
            result =0;
    }
    
    if(result){
        sendUartStr((char *)"(OK)");
    }
    else{
        sendUartStr((char *)"(ERR)");
    }

}

void sendUartCh(char ch)
{
    while (!TXIF);
    TXREG = ch;
}

void sendUartStr(char *str)
{
    int i=0;

    while(str[i]!='\0'){
        sendUartCh(str[i++]);
    }
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

	PwmDuty = 0x20;
	PWM1DCH = PwmDuty;


}

void init_uart(void)
{
        // 115200bps
	BAUDCONbits.BRG16 = 1;
	TXSTAbits.SYNC = 0;
	TXSTAbits.BRGH = 1;
	SPBRGL = 34;

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


