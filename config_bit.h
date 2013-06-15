/* 
 * File:   config_bit.h
 * Author: A15153
 *
 * Created on 2013/02/05, 15:17
 */

#ifndef CONFIG_BIT_H
#define	CONFIG_BIT_H

#ifdef	__cplusplus
extern "C" {
#endif


// PIC16LF1509 Configuration Bit Settings

#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = ON   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


void putBuf(char ch);
void clearBuf(void);
int chkBuf(void);
int addChkBuf(char ch);

enum CMD_NUM {
    CMD_NUM_ON_LED1 = 0,
    CMD_NUM_ON_LED2,
    CMD_NUM_ON_LED3,
    CMD_NUM_OFF_LED1,
    CMD_NUM_OFF_LED2,
    CMD_NUM_OFF_LED3,
    CMD_NUM_UP_PWM1,
    CMD_NUM_DOWN_PWM1,

};



#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_BIT_H */

