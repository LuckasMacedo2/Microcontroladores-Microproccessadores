/*
 * File:   ex8.c
 * Author: Lucas
 *
 * Created on 4 de Março de 2019, 23:33
 */


#include "config.h"

void inicializa_leds(){
    PORTB = 0b00000000;
    PORTC = 0b00000010;
    PORTD = 0b00000000;
    
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    return;
}

void conta_PORTB(int n){
    for (int i = 0; i < n; i++){
        PORTB = i;
       __delay_ms(150);
       PORTB = 0x0;
       __delay_ms(150);
    }
    return;
}

void liga_rele(){
    PORTC = 0b00000011;
    __delay_ms(150);
    return;
}

void desliga_rele(){
    PORTC = 0b00000010;
    return;
}

void beep(){
    PORTC = 0b00000000;
    __delay_ms(150);
    PORTC = 0b00000010;
    return;
}

void main(void) {
    while (1){
        inicializa_leds();
        
        // Liga o relé
        liga_rele();
        
        // Realiza a ocntagem até 12
        conta_PORTB(12);
        
        // Desliga o relé
        desliga_rele();
        
        // Beep no buzzer
        beep();
    }
    return;
}
