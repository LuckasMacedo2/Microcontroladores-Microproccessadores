/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva um 
 * programa para o
 *  PIC18F4520 que execute o seguinte procedimento:
 * acione os LED RD4 a RD7 do circuito, enquanto os demais permanecem desligados;
 * os LED do PORTB devem permanecer todos desliga- dos.
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#include <xc.h>

void inicializa()
{
    PORTD = 0;  // Escreve e le o nível dos pinos de uma porta
    TRISD = 0;  // Configura o sentido de fluxo de uma porta
                // 0 -> Saida
                // 1 -> Entrada
    LATD = 0;   // Armazena o valor do ultimo comando de escrita    
}

void main(void) {
    inicializa();
    
    while (1)
        LATD = 0b11110000;
    return;
}
