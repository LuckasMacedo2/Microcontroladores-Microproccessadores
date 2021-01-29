/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Desenvolva um programa que execute um sequencial de LED no PORTD do
 * PIC18F4520, de forma que o LED em RD0 seja acionado e, em seguida, o LED em
 * RD1 e assim sucessivamente. Ao acionar o LED sucessivo, o anterior deve ser apagado.
 * O tempo de acionamento de cada LED e´ de 0,5 segundo. O sequencial deve ocorrer do
 * LED em RD0 até o LED em RD5. O LED conectado em RD6 deve permanecer sempre
 * apagado. Pore´m, o LED conectado em RD7 deve piscar de forma que fique 2 segundos
 * aceso e 2 segundos apagado, ou seja, com 0,25Hz. O acionamento do RD7 deve ocorrer
 * ao mesmo tempo do sequencial de RD0 até RD5. Os LED do PORTB permanecem
 * desligados durante todo o programa.
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
    PORTB = 0;
    TRISB = 0;
    LATB = 0;
    
    PORTC = 0;
    TRISC = 0;
    LATC = 0;
    
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
}


void main(void) {
    inicializa();
    
    int on = 0;
    int controle = 0;
    int i = 0;
    
    while (1){
        
        if (controle == 4){
            if (on == 0)
                on = 1;
            else
                on = 0;
            controle = 0;
        }
        
        LATD = 128 * on + pow(2, i);
        __delay_ms(500);
        
        
        i ++;
        controle ++;
        if (i == 6)
            i = 0;
    }
    return;
}
