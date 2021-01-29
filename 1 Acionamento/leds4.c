/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Utilizando o IDE MPLAB X em conjunto com o com- pilador XC8, desenvolva um 
 * programa para o PIC18F4520, considerando o hardware ilustrado na Figura1, 
 * que execute o seguinte procedimento:
 * inicialmente, mantenha todos os LED do PORTB e do PORTD desligados;
 * implemente um sequencial de LED que acione do LED 0 do PORTD até o LED 7 do
 * PORTD. Ao acionar um LED, o anterior deve ser desligado. Utilize uma temporização
 * de um quarto de segundo.
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#include <math.h> // funcao pow())

void inicializa()
{
    PORTD = 0;  // Escreve e le o nivel dos pinos de uma porta
    TRISD = 0;  // Configura o sentido de fluxo de uma porta
                // 0 -> Saida
                // 1 -> Entrada
    LATD = 0;   // Armazena o valor do ultimo comando de escrita 
    
    PORTB = 0;
    TRISB = 0;
    LATB = 0;
}


void sequencial(){
    for (int i = 8; i >= 0; i --){
        LATD = pow(2, i);
        __delay_ms(250);
    }
}

void main(void) {
    inicializa();
    
    while (1){
        sequencial();
    }
    return;
}
