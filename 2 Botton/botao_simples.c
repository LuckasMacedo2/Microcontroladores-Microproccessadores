/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Apertar um botao e acender o led equivalente em RD
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
    TRISB = 0b11111111;         // Todo mundo como entrada
    LATB = 0;
   
    
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
    
    INTCON2bits.RBPU = 0;       // Liga os resistores de poll-up
    ADCON1 = 0b00001111;        // Define como entrada digital, todos os ports ANn 
}

int valor(){
    if (PORTBbits.RB0 == 0)    // Botao 1
        return 1;
    if (PORTBbits.RB1 == 0)    // Botao 2
        return 2;
    if (PORTBbits.RB2 == 0)    // Botao 3
        return 4;
    if (PORTBbits.RB3 == 0)    // Botao 4
        return 8;
    if (PORTBbits.RB4 == 0)    // Botao 5
        return 16;
    if (PORTBbits.RB5 == 0)    // Botao 6
        return 32;
    if (PORTBbits.RB6 == 0)    // Botao 7
        return 64;
    if (PORTBbits.RB7 == 0)    // Botao 8
        return 128;
    return 0;
}

void main(void) {
    inicializa();
    
    int x;
    
    while (1){
       
        x = valor();
        if (x != 0){
            LATD = x;
            __delay_ms(500);
            LATD = 0;
        }   
    }
}
