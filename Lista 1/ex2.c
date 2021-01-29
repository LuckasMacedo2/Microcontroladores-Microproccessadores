/*
 * File:   ex2.c
 * Author: Lucas
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva um programa para
 * o PIC18F4520, que execute o seguinte procedimento:
 * para o PORTD, manter acionados os LED cujos bits são ímpares. 
 * Os demais devem permanecer desligados;
 * para o PORTB, manter acionados os LED cujos bits são pares.
 * Os demais devem permanecer desligados.
 * Created on 3 de Março de 2019, 21:22
 */


#include "config.h"

void main(void) {
    TRISD = 0b00000000; //Configurando como saída
    TRISB = 0b00000000; //Configurando como saída
    
    PORTD = 0b00000000; //Zerando a porta
    PORTB = 0b00000000; //Zerando a porta
    
    
    while(1){
        PORTB = 0b01010101;
        PORTD = 0b10101010;
    }
    return;
}
