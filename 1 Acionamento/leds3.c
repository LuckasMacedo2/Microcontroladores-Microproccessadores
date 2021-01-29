/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Utilizando o IDE MPLAB X em conjunto com o compilador XC8, desenvolva um 
 * programa para o PIC18F4520 , que execute o seguinte procedimento:
 * inicialmente, mantenha todos os LED do circuito desligados;
 * piscar o LED conectado ao pino 22 do CI (circuito integrado) 5 vezes. 
 * O tempo de acionamento é de 0,4 segundos. Ou seja, 
 * o LED permanece 400ms LIGADO e 400ms DESLIGADO em cada piscada. Todos os demais 
 * LED do circuito deve permanecer desligados.
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000

#include <xc.h>

void inicializa()
{
    PORTD = 0;  // Escreve e le o nível dos pinos de uma porta
    TRISD = 0;  // Configura o sentido de fluxo de uma porta
                // 0 -> Saida
                // 1 -> Entrada
    LATD = 0;   // Armazena o valor do ultimo comando de escrita    
}

#define LED3 LATDbits.LATD3

void main(void) {
    inicializa();
    
    while (1){
        LED3 = 1;
        __delay_ms(400);
        LED3 = 0;
        __delay_ms(400);
    }
    return;
}
