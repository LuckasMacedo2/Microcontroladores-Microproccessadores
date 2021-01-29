/*
 * File:   ex10.c
 * Author: Lucas
 *
 * Created on 5 de Março de 2019, 19:40
 */


#include "config.h"

void inicializa(){
    PORTB = 0b00000000;
    PORTD = 0b00000000;
    
    TRISB = 0b00000000;
    TRISD = 0b00000000;
    return;
}


void main(void) {
    
    inicializa();
    
    while (2){
        PORTD = 0b00100001;
        __delay_ms(8000);
        PORTD = 0b00010010;
        __delay_ms(2000);
        PORTD = 0b00001100;
        __delay_ms(8000);

    }
    
    return;
}
