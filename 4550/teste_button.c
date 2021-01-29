/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 9 de Junho de 2019, 11:51
 */

#include "configuracao.h"

#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
#define C4 PORTBbits.RB3


void cfg(){
    TRISB = 0b00001111;
    
    TRISC = 0;
    
    OSCCON=0b01100000;
    
    INTCON2bits.RBPU = 0;
}

#define LED LATCbits.LATC7

void led(int n){
    for(int i = 0; i <= n; i ++){
        LED = 1;
        __delay_ms(100);
        LED = 0;
        __delay_ms(100);
    }
}

void main(void) {
    cfg();
    while(1){
        if (C1 == 0){
            led(3);
        }
        if (C2 == 0){
            led(4);
        }
        if (C3 == 0){
            led(5);
        }
        if (C4 == 0){
            led(6);
        }
    }
}
