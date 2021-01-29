/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Contar em 1 display apertar o botao (RB0) e contar no outro display
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

#define DISP1 LATAbits.LATA2    // Display 1
#define DISP2 LATAbits.LATA3    // Display 2
#define DISP3 LATAbits.LATA4    // Display 3
#define DISP4 LATAbits.LATA5    // Display 4

int display = 1;
int valor = 0;

void configurar_interrupcoes(){
    RCONbits.IPEN = 1;          // Habilita as interrupcoes externas
    INTCONbits.GIEH = 1;        // Habilita interrupcoes de alta prioridade
    
    // RB0
    INTCONbits.INT0IE = 1;      // Habilita interrupcao em RB0
    INTCONbits.INT0IF = 0;      // Indica que ainda nao ocorreu a interrupcao em RB0
    INTCON2bits.INTEDG0 = 0;    // Indica que ocorrera a interrupcao na borda de descida
}

// Funcao para tratar interrupcoes
void __interrupt(high_priority) isr_alta(){
    switch (display){
        case 1:
            DISP1 = 0;
            DISP2 = 1;
            display = 2;
            break;
        case 2:
            DISP2 = 0;
            DISP3 = 1;
            display = 3;
            break;
        case 3:
            DISP3 = 0;
            DISP4 = 1;
            display = 4;
            break;
        case 4:
            DISP3 = 0;
            DISP4 = 1;
            display = 5;
            break;
        case 5:
            DISP1 = 1;
            DISP4 = 0;
            display = 1;
            break;
    }
	INTCONbits.INT0IF = 0;      // Reseta a flag
}


void inicializa()
{
    ADCON1 = 0b00001111;        // Todas as portas ANn como digitais
    
    // Controle dos displays
    PORTA = 0;
    LATA = 0;
    TRISA = 0;
    DISP1 = 1;
    
    // Controle dos segmentos do display de 7 segmentos
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
    
    // Controle dos botoes
    PORTB = 0;
    LATB = 0;
    TRISB = 1;                  // RB0 como entrada
}

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

void contagem(int n){
    for (; valor < n; valor ++){
        LATD = int_to_led(valor);
        __delay_ms(400);
    }
    valor = 0;
}




void main(void) {
    inicializa();
    configurar_interrupcoes();
    while(1){
        contagem(15);
    }
}
