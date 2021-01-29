/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Usando o IDE MPLAB X, desenvolva um programa para o microcontrolador
 * PIC18F4520 que mantenha todos os LED do circuito desligados. Em seguida, piscar o
 * LED em RD0 por três vezes, com frequência de 2 Hz. Ao final do acionamento do
 * LED, acionar o motor (conectado em RC2) e, em seguida, acionar os LED do PORTB
 * em sequência, do RB0 ao RB7, com tempo de 0,35 segundo. A Figura 1 ilustra o
 * esquema de ligação do motor.
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
    
    PORTC = 0;
    TRISC = 0;
    LATC = 0;
}

#define LED0 LATDbits.LATD0
#define MOTOR LATCbits.LATC2


void sequencial(int inicio, int passo, int fim){
    for (int i = inicio; i != fim; i += passo){
        LATB = pow(2, i);
        __delay_ms(350);
    }
    LATB = 0;
}

void pisca3vezes(){
    for (int i = 0; i < 3; i ++){
        LED0 = 1;
        __delay_ms(500);
        LED0 = 0;
        __delay_ms(500);
    }
    LED0 = 0;
}



void main(void) {
    inicializa();
    
    while (1){
        pisca3vezes();
        MOTOR = 1;
        sequencial(0, 1, 8);
        MOTOR = 0;
    }
    return;
}
