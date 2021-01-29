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
    
    TRISD = 0;
    
    TRISC = 0;
    
    OSCCON=0b01100000;
    
    INTCON2bits.RBPU = 0;
}

int varrer(){
    int valor = -1;
    LATD = 0b00001110;
    if (C1 == 0){
        valor = 14;
    }
        if (C2 == 0){
        valor = 13;
    }
    if (C3 == 0){
        valor = 12;
    }
    if (C4 == 0){
        valor = 11;
    }
    
    LATD = 0b00001101;
    if (C1 == 0){
        valor = 10;
    }
        if (C2 == 0){
        valor = 9;
    }
    if (C3 == 0){
        valor = 6;
    }
    if (C4 == 0){
        valor = 3;
    }
    
    LATD = 0b00001011;
    if (C1 == 0){
        valor = 0;
    }
        if (C2 == 0){
        valor = 8;
    }
    if (C3 == 0){
        valor = 5;
    }
    if (C4 == 0){
        valor = 2;
    }
    
    LATD = 0b00000111;
    if (C1 == 0){
        valor = 15;
    }
        if (C2 == 0){
        valor = 7;
    }
    if (C3 == 0){
        valor = 4;
    }
    if (C4 == 0){
        valor = 1;
    }
    
    PORTB = 0;
    return valor;
}

#define LED LATCbits.LATC7

void led(int n){
    for(int i = 0; i < n; i ++){
        LED = 1;
        __delay_ms(100);
        LED = 0;
        __delay_ms(100);
    }
}

void testa_led(){
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
void main(void) {
    cfg();
    int x = -1;
    while(1){
        
        x = varrer();
        if (x != -1)
        {
            led(x);
        }
    }
}
