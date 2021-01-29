/*
 * File:   ex3.c
 * Author: Lucas
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva
 * um programa para o PIC18F4520 , que execute o seguinte procedimento:
 * inicialmente, mantenha todos os LED do circuito desligados;
 * piscar o LED conectado ao pino 22 do CI (circuito integrado) 5 vezes.
 * O tempo de acionamento é de 0,4 segundos. Ou seja, o LED permanece 400ms LIGADO e 
 * 400ms DESLIGADO em cada piscada. Todos os demais LED do circuito deve permanecer desligados.
 * Created on 3 de Março de 2019, 21:33
 */

// Pino 22 = RD3/PSP3

#include "config.h"

void main(void) {
    
    TRISD = 0b00000000; // Setando como saida
    TRISB = 0b00000000; // Setando como saida
    
    PORTD = 0b00000000; // Zerando a porta
    PORTB = 0b00000000; // Zerando a porta

    
    while(1){
        __delay_ms(1000);
        for (int i = 0; i < 5; i ++){
            PORTD = 0b00001000;
            __delay_ms(400);
            PORTD = 0b00000000;
            __delay_ms(400);
        }
    }
    
    return;
}
