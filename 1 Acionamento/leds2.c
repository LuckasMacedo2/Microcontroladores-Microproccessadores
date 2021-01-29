/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva um programa para o PIC18F4520,
 *  que execute o seguinte procedimento:
 * para o PORTD, manter acionados os LED cujos bits são ímpares. Os demais devem permanecer desligados;
 * para o PORTB, manter acionados os LED cujos bits são pares. Os demais devem permanecer desligados.
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
        LATD = 0b10101010;
    return;
}
