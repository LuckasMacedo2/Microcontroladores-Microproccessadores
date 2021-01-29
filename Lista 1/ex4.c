/*
 * File:   ex4.c
 * Author: Lucas
 * Utilizando o IDE MPLAB X em conjunto com o com- pilador XC8, desenvolva um programa para o 
 * PIC18F4520, considerando o hardware ilustrado na Figura1, que execute o seguinte procedimento:
 * inicialmente, mantenha todos os LED do PORTB e do PORTD desligados;
 * implemente um sequencial de LED que acione do LED 0 do PORTD até o LED 7 do PORTD. 
 * Ao acionar um LED, o anterior deve ser desligado. 
 * Utilize uma temporização de um quarto de segundo.
 * Created on 3 de Março de 2019, 21:49
 */


#include "config.h"

#include <math.h>

void main(void) {
    // Utilizando decimais
    TRISD = 0x0; // Setando como saida
    TRISB = 0x0; // Setando como saida
    
    PORTD = 0x0; // Zerando a porta
    PORTB = 0x0; // Zerando a porta
    
    
    
    while(1 + 1 == 2){
        for (int i = 0; i < 8; i++){
            PORTD = pow(2,i);
            __delay_ms(250);
        }
        
    }
    
    return;
}
