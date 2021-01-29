/*
 * File:   ex1.c
 * Author: Lucas
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva um programa para 
 * o PIC18F4520 que execute o seguinte procedimento:
 * acione os LED RD4 a RD7 do circuito, enquanto os demais permanecem desligados;
 * os LED do PORTB devem permanecer todos desliga- dos.
 * 
 * 
 * Created on 3 de Março de 2019, 20:53
 */


#include "config.h"
void main(void) {
    TRISD = 0b00000000; //Configurando como saída
    TRISB = 0b00000000; //Configurando como saída
    
    PORTD = 0b00000000; //Zerando a porta
    PORTB = 0b00000000; //Zerando a porta
    
    
    while(1){
        PORTD = 0b11110000;
    }
    return;
}
