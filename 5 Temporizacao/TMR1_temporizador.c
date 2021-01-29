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
 Timer1: Possui um oscilador interno, pinos (RC1 e RC3).
 * Quando configurada como contador o Timer1 pode ser contador sincrono ou assincrono
 * 
 * T1CON = 0b76543210
 * 7 -> RD16 habilita 16 bits) 1 = 16 bits; 0 = 8 bits
 * 6 -> T1RUN status do clock) 1 = clock derivado do oscilador do Timer1;
 *                             0 = clock derivado de outra fonte
 * 5:4 -> T1CKPS1:T1CKPS0) Selecao do prescaler
 * 3 -> T1SYNC\) 
 *                  Se TM1CS = 1: 1 = nao sincroniza com o clock externo; 0 = sincroniza com o clock externo
 *                  Se TM1CS = 0: Ignora
 * 2 -> T1OSCEN) Habilita o oscilador do Timer1: 1 = habilitado; 0 = desabilitado
 * 1 -> TMR1CS) Selecao do incremento do TMR1: 1 = incrementado pelo sinal em RC3 (borda de subida)
 *                                             0 =  incrementado pelo ciclo de instrucao
 * 0 -> TMR1ON) Liga o modulo: 1 = liga. 0 = desliga
 * 
 */ 

// Exemplo de configuracao TMR1
void configura_TMR1(){      // bit
    T0CON = 0b10110001;     // 7: RD16 TMR1 como 16 bits
                            // 6: T1RUN clock derivado de outra fonte
                            // 5:4: T1CKPS1:T1CKPS0 prescaler de 1:8 
                            // 3: T1SYNC sincronizado
                            // 2: T1OSCEN oscilador do Timer1 desligado
                            // 1: TMR1CS Timer1 incrementado pelo ciclo de instrucao
                            // 0: TMR1ON Timer1 ligado
    TMR1L = 200;            // Inicializa o TMR1
    TMR1H = 300;            // Inicializa o TMR1
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
    T0CON = 0b10110000;     // <7>: RD16 TMR1 como 16 bits
                            // <6>: T1RUN clock derivado de outra fonte
                            // <5:4>: T1CKPS1:T1CKPS0 prescaler de 1:8 
                            // <3>: T1SYNC sincronizado
                            // <2>: T1OSCEN oscilador do Timer1 desligado
                            // <1>: TMR1CS Timer1 incrementado pelo ciclo de instrucao
                            // <0>: TMR1ON Timer1 desligado
    
    int x;
    
    inicializa();
    TMR1L = 0;
    TMR1H = 0;
    
    while(1){
        if(botao == 0){
            LED = 1;
            T1CONbits.TMR1ON = 1; // Liga o TMR1
            while(botao == 0);
            T1CONbits.TMR1ON = 0;
            LED = 0;
            x = TMR1H<<8;
            x = x | TMR1H;
            TMR1L = 0;
            TMR1H = 0;
            imprimir_display(x/1000, (x%1000)/100, (x%100)/10, x%10);
        }
    }
}