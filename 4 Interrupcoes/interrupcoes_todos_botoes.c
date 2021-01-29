/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Piscar os leds em RD, ao pressionar o botao RB2 baixa prioridade
 * contar ate 15, ao apertar RB0 alta prioridade ligar o motor por 1 s e 
 * depois desligar ele
 * apertat RB1 alta prioridade liga o Rele (RC0) por 1 s 3 vezes
 */

/* Interrupcao
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#define BUTTON1 LATBbits.LATB0  // Botao 0
#define BUTTON2 LATBbits.LATB1  // Botao 1
#define BUTTON3 LATBbits.LATB2  // Botao 2

#define DISP1 LATAbits.LATA2    // Display 1

#define MOTOR LATCbits.LATC2    // Motor

#define RELE LATCbits.LATC0

const int int_to_led(int index  ){
    const int v[16] = {
        63,     // 0
        6,      // 1
        91,     // 2
        79,     // 3
        102,    // 4
        109,    // 5
        125,    // 6
        7,      // 7
        127,    // 8
        111,    // 9
        119,    // A 
        124,    // B
        57,     // C
        94,     // D
        121,    // E
        113     // F
    };
    
    return v[index];
}

void configurar_interrupcoes(){
    RCONbits.IPEN = 1;          // Habilita as interrupcoes externas
    INTCONbits.GIEH = 1;        // Habilita interrupcoes de alta prioridade
    INTCONbits.GIEL = 1;        // Habilita baixas prioridades
    
    // RB0
    INTCONbits.INT0IE = 1;      // Habilita interrupcao em RB0
    INTCONbits.INT0IF = 0;      // Indica que ainda nao ocorreu a interrupcao em RB0
    INTCON2bits.INTEDG0 = 0;    // Indica que ocorrera a interrupcao na borda de descida
    
    // RB1
    INTCON3bits.INT1IF = 0;     // Seta a flag de RB1 como 0
    INTCON2bits.INTEDG1 = 0;    // Borda de descida
    INTCON3bits.INT1IE = 1;     // Habilita interrupcao em RB1
    
    INTCON3bits.INT1IP  = 1;    // Habilita RB1 como alta prioridade
    
    // RB2
    INTCON3bits.INT2IF = 0;     // Seta a flag de RB2 como 0
    INTCON2bits.INTEDG2 = 0;    // Borda de descida
    INTCON3bits.INT2IE = 1;     // Habilita interrupcao em RB2
    
    INTCON3bits.INT2IP  = 0;    // Habilita RB2 como baixa prioridade
}

// Funcao para tratar interrupcoes
// Altas prioridades
void __interrupt(high_priority) isr_alta(){
    if (INTCONbits.INT0IF == 1){
        for (int i = 0; i < 5; i++){
            MOTOR = 1;
            __delay_ms(1000);
            MOTOR = 0;
        }
    }else{
        if (INTCON3bits.INT1IF == 1){
            for (int i = 0; i < 5; i ++){
                RELE = 1;
                __delay_ms(1000);
                RELE = 0;
            }
        }
    }
    // Indica que a interrupcao ja foi tratada 
    INTCONbits.INT0IF = 0; 
    INTCON3bits.INT1IF = 0;      
}

// Baixas prioridades
void __interrupt(low_priority) isr_low(){
    DISP1 = 1;
    for (int i = 0; i < 15; i++){
        LATD = int_to_led(i);
        __delay_ms(500);
    }
    DISP1 = 0;
    INTCON3bits.INT2IF = 0;      
}

void inicializa()
{
    ADCON1 = 0b00001111;        // Todas as portas ANn como digitais
    
    // Controle dos displays
    PORTA = 0;
    LATA = 0;
    TRISA = 0;
        
    // Controle dos segmentos do display de 7 segmentos
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
    
    // Controle dos botoes
    PORTB = 0;
    LATB = 0;
    TRISB = 0b00000111;                  // RB0 como entrada
    
    // Controle do motor
    PORTC = 0;
    LATC = 0;
    TRISC = 0;
}

void main(void) {
    inicializa();
    configurar_interrupcoes();
    while(1){

    }
}
