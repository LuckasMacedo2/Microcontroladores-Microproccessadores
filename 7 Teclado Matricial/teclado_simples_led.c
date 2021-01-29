#pragma config WDT = OFF
#pragma config OSC = HS
#pragma config MCLRE = ON
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000

#include <xc.h>

/*
 * Periodo do PWM = 500 ms
 * Pwm periodo = [(PR2 + 1)] * 4 * Tosc * Prescaler
 * PR2 = 249 
 */

// *************************** LCD ***************************************************
#include <stdio.h>
#define LINHA1 128
#define LINHA2 192

#define RS LATEbits.LATE2       // Indica se e dado ou instrucao
#define E LATEbits.LATE1        // Pino que inicia o ciclo leitura/escrita
//#define RW LATEbits.LATE2       // Indica se e leitura ou escrita

void comando(int cmd){          // Executa um comando
    RS = 0;                     // Indica que e uma instrucao
    E = 1;                      // Pulso
    LATD = cmd;                 // Envia o comando para o barramento de dados DB7:DB0
    __delay_ms(2);
    E = 0;                      // Fim do pulso
}

void limpar_LCD(){              // Limpa o LCD
    comando(1);
}

void inicializa_LCD(){
    E = 0;                      // Finaliza o ciclo de leitura escrita
    RS = 0;                     // Indica uma instrucao
    comando(0b00111100);        // Inicializa o display
    comando(0b00001100);        // Liga o lcd
    limpar_LCD();               // Limpa o LCD
}

void enviar_dados_LCD(int dado){    // Envia os dados para o LCD
    RS = 1;                         // Indica que um dado sera escrito em DB7:DB0
    E = 1;                          // Inicia o ciclo de escrita
    LATD = dado;                    // Envia os dados para DB7:DB0
    __delay_ms(20);                  // Espera
    E = 0;                          // Finaliza o ciclo de escrita
}

void imprimir_dados_LCD(char dados[]){
    int i = 0;                      // Contador
    while(dados[i] != 0){
        enviar_dados_LCD(dados[i]);
        i++;
    }
}
// *************************** LCD ***************************************************

// *************************** TECLADO ***************************************************
#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2

int varredura(){
    int r = -1;
    
    LATD = 0b00001110;
    if  (C1 == 0){
        r = 1;
    }
    if  (C2 == 0){
        r = 2;
    }
    if  (C3 == 0){
        r = 3;
    }
    
    LATD = 0b00001101;
    if  (C1 == 0){
        r = 4;
    }
    if  (C2 == 0){
        r = 5;
    }
    if  (C3 == 0){
        r = 6;
    }
    
    LATD = 0b00001011;
    if  (C1 == 0){
        r = 7;
    }
    if  (C2 == 0){
        r = 8;
    }
    if  (C3 == 0){
        r = 9;
    }
    
    LATD = 0b00000111;
    if  (C1 == 0){
        r = 10;
    }
    if  (C2 == 0){
        r = 0;
    }
    if  (C3 == 0){
        r = 11;
    }
    
    return r;
}
// *************************** TECLADO ***************************************************

void cfg(){
    PORTD = 0;
    LATD = 0;
    TRISD  = 0;
    
    PORTE = 0;
    LATE = 0;
    TRISE = 0;
    
    PORTB = 0;
    LATB = 0;
    TRISB = 0b00000111;
    
    // Portas como digitais
    ADCON1 = 0b00001111;
    
    // Habilitando resistores de pull up
    INTCON2bits.RBPU = 0;
}

#define LED LATBbits.LATB7

void main(){
    cfg();
    inicializa_LCD();
    
    int i = -1;
    
    comando(LINHA1);
    imprimir_dados_LCD("Teclado matricial");
    char c[5];

    while(1){
        i = varredura();
        
        if (i != -1){
            sprintf(c, "%d", i);
            comando(LINHA2);
            imprimir_dados_LCD(c);
            
            for (int j = 0; j < i; j ++){
                LED = ~LED;
                __delay_ms(10);
            }
        }
    }
}