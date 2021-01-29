/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Escreva um programa para o microcontrolador PIC18F4520 que acione o rele´ (conectado
 *em RC0) e efetue uma contagem em bina´rio no PORTB. Ou seja, a contagem deve seguir
 * o padrao: 0000, 0001, 0010 e assim sucessivamente. A Figura 3 ilustra o diagrama de
 * ligação do rele.
 * Quando a contagem atingir o valor 12 (em decimal), desligar o rele´ dar um beep no buzzer
 * com tempo de 0,15 segundo. Embora no diagrama esquema´tico o buzzer esteja conectado
 * ao RC1 via porta NOT, no kit dida´tico o buzzer e´ acionado via transistor PNP, mantendo
 * a logica invertida para seu funcionamento.
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#include <math.h> // funcao pow())

#define LED0 LATDbits.LATD0
#define RELE LATCbits.LATC0
#define BUZZER LATCbits.LATC1

void inicializa()
{
    PORTB = 0;
    TRISB = 0;
    LATB = 0;
    
    PORTC = 0;
    TRISC = 0;
    LATC = 0;
    BUZZER = 1;
}


void pisca_bin(){
    for (int i = 0; i <= 12; i ++){
        LATB = i;
        __delay_ms(250);
        LATD = 0;
        __delay_ms(250);
    }
}

void beep(){
    BUZZER = 0;
    __delay_ms(150);
    BUZZER  = 1;
    __delay_ms(150);
}

void main(void) {
    inicializa();
    
    while (1){
        RELE = 1;   // Liga o rele
        pisca_bin();
        RELE = 0;   // Desliga o rele
        beep();        
    }
    return;
}
