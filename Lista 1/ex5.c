/*
 * File:   ex5.c
 * Author: Lucas
 * Complemente o programa anterior de forma que, após o sequencial de LED do 
 * PORTD, outro sequencial seja implementado no PORTB no sentido contrário, 
 * ou seja, do LED 7 ao LED 0. 
 * Empregue a mesma temporização do exercício anterior e mantenha seu 
 * programa executando em laço infinito.
 * Created on 3 de Março de 2019, 22:02
 */

#include "config.h"

#include <math.h>

void main(void) {
    // Utilizando decimais
    TRISD = 0x0; // Setando como saida
    TRISB = 0x0; // Setando como saida
    
    PORTD = 0x0; // Zerando a porta
    PORTB = 0x0; // Zerando a porta
    
    
    
    while(1){
        for (int i = 0; i < 8; i++){
            PORTD = pow(2,i);
            __delay_ms(250);
        }
        
        PORTD = 0;
        
        for (int i = 8; i >= 0; i--){
            PORTB = pow(2,i);
            __delay_ms(250);
        }
        
        PORTB = 0;
    }
    
    return;
}
