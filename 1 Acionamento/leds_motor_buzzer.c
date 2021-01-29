/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Complemente o programa do exercício anterior que, ao final do procedimento, ou seja,
 * quando o motor é desligado, um pulso de duração de 0,2 segundo e´ aplicado ao buzzer
 * do circuito. Perceba na Figura 2 que o buzzer está conectado ao RC1 com lógica invertida,
 * ou seja:
 * nıvel logico alto (1) LIGA O BUZZER;
 * nıvel logico baixo (0) DESLIGA O BUZZER
 * Desta forma, logo no início do programa o I/O RC1 deve ser colocado em n´ıvel alto para
 * que o buzzer na˜o seja acionado de maneira incorreta.
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#include <math.h> // funcao pow())

#define LED0 LATDbits.LATD0
#define MOTOR LATCbits.LATC2
#define BUZZER LATCbits.LATC1

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
    
    MOTOR = 0;
    BUZZER = 1;
}


void sequencial(int inicio, int passo, int fim){
    LATB = 1;
    __delay_ms(350);
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


void buzzer(){
    LED0 = 1;
    BUZZER = 0;
    __delay_ms(200);
    BUZZER = 1;
    __delay_ms(200);
    LED0 = 0;
}

void main(void) {
    inicializa();
    
    while (1){
        pisca3vezes();
        MOTOR = 1;
        sequencial(0, 1, 8);
        MOTOR = 0;
        buzzer();
    }
    return;
}
