/*
 * File:   ex7.c
 * Author: Lucas
 *
 * Created on 4 de Março de 2019, 21:45
 */


#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#include <math.h>

void inicializa_leds(){
    PORTB = 0b00000000;
    PORTC = 0b00000010;
    PORTD = 0b00000000;
    
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    return;
}

void pisca_RD0(int n){
    for (int i = 0; i < n; i ++){
        PORTD = 0b0000001;
        __delay_ms(500);
        PORTD = 0b0000000;
        __delay_ms(500);
    } 
    return;
}

void liga_motor(){
    PORTC = 0b00000110;
    return;
}

void desliga_motor(){
    PORTC = 0b00000010;
    return;
}

void pisca_RB_seq(){
    PORTB = 0x0;
    for (int i = 0; i<8; i ++){
        PORTB = pow(2,i);
        __delay_ms(350);
        PORTB = 0x0;        
    }
    return;
}

void liga_buzzer(){
   
    PORTC = 0b00000000;
    __delay_ms(200);
    PORTC = 0b00000010;

    return;
}

void main(void) {
    while(1){
        // Leds desligados
        inicializa_leds();
        
        // 2 hz = 0.5 s = 0.5*1000 = 500ms
        pisca_RD0(3);
        
        // Aciona o motor
        liga_motor();
                
        // Pisca os ledes em sequencia de PORTB
        // 0.35s = 0.35*1000 = 350ms
        pisca_RB_seq();
        
        // Desliga o motor
        desliga_motor();
       // 
        //Liga o buzzer
        liga_buzzer();
    }
    
    return;
}
