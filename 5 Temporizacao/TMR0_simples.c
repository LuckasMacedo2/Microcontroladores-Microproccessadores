/*
 * File:   newmain.c
 * Author: Lucas
 *
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000

#include <xc.h>

/*
 Timer0: Pode ser contador ou temporizador 
 *      -> Contador: Conta os pulsos aplicados em RA4
 *      -> Temporizador: Conta os ciclos de instrucao, conta os ciclos de instrucao
 *                      gerados pelo oscilador. Prescaler retarda o incremento de TMR0
 *
 *  TMR0L (8 bits) ou TMR0L e TMR0H (16 bits) estoura, quando ocorre o estouro os registradores
 * sao zerados e a contagem reiniciada.
 *  *** O programa nao precisa ficar parado para o tempo passar
 
 * T0CON = 0b7654321
 * 7 -> TM0ON: Liga o TMR0) 1 = Liga; 0 = Desliga.
 * 6 -> T08BIT: Seleciona o modo de operacao) 1 = 8 bits; 0 = 16 bits.
 * 5 -> T0CS: Incremento do TMR0) 1 = Contador, sera incrementado pelos pulsos aplicados em RA4;
 *                                0 = Temporizador, conta os ciclos de intrucao
 * 4 -> T0SE: Borda de incremento) 1 = Borda de descida; 0 = Borda de subida
 * 3 -> PSA: Habilita o prescaler) 1 = Desativa; 0 = Ativa
 * 2:0 -> T0PSA2:T0PSA0: Fator do prescaler
 * 
 * Aperta um botao e contar o tempo e enviar para o display de 7 segmentos
 * 
 */ 

void configura_TMR0(){      // bit
    T0CON = 0b11000100;     // 7: TMR0ON ligado
                            // 6: T08BIT configurado como 8 bits
                            // 5: TMR0 como temporizador  
                            // 4: T0SE borda de incremento, subida
                            // 3: PSA prescaler ativado
                            // 2:0: Prescaler configurado resolucao 1:32
    TMR0L = 6;              // Inicializa o TMR0
    // Numeros de incremento antes do estouro
    // N_inc = 256 - valor_inicial
    // Tempo de estouro
    // TE = N_inc * (4/Fosc) * prescaler
}

#define botao PORTBbits.RB0

#define LED LATBbits.LATB7

#define DISP1 LATAbits.LATA2
#define DISP2 LATAbits.LATA3
#define DISP3 LATAbits.LATA4
#define DISP4 LATAbits.LATA5


void inicializa(){
    PORTB = 0;
    LATB = 0;
    TRISB = 1;              // RB0 como entrada
    
    PORTA = 0;
    LATA = 0;
    TRISA = 0;
    
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
    
    ADCON1 = 0b00001111;    // Seta as portas como digitais
    INTCON2bits.RBPU = 0;   // Habilita os resistores de pull up
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

void imprimir_display(int milhar, int centena, int dezena, int unidade){
    for (int i = 0; i < 313; i++){
        DISP1 = 1;
        LATD = int_to_led(milhar);
        __delay_ms(4);
        DISP1 = 0;
        
        DISP2 = 1;
        LATD = int_to_led(centena);
        __delay_ms(4);
        DISP2 = 0;
        
        DISP3 = 1;
        LATD = int_to_led(dezena);
        __delay_ms(4);
        DISP3 = 0;
        
        DISP4 = 1;
        LATD = int_to_led(unidade);
        __delay_ms(4);
        DISP4 = 0;
    }
}

void main(){
    T0CON = 0b01000010;     // <7> TRM0ON: TMR0 desligado
                            // <6> T08BIT: TMR0 com 8 bits de resolucao
                            // <5> T0CS: TMR0 como temporizador
                            // <4> T0SE: Borda de subida
                            // <3> PSA: Prescaler ativado
                            // <2:0> T0PS2:T0PS0: Fator do prescaler = 1:8
    
    int x;
    
    inicializa();
    
    while(1){
        if(botao == 0){
            LED = 1;
            T0CONbits.TMR0ON = 1; // Liga o TMR0
            while(botao == 0);
            T0CONbits.TMR0ON = 0;
            LED = 0;
            x = TMR0L;
            TMR0L = 0;
            imprimir_display(x/1000, (x%1000)/100, (x%100)/10, x%10);
        }
    }
}