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


void cfg(){
    LATD = 0;
    TRISD = 0;
    PORTD = 0;
    
    LATE = 0;
    PORTE = 0;
    TRISE = 0;
    
    LATC = 0;
    PORTC = 0;
    TRISC = 0;
}

void cfg_PWM(){
    CCP1CON = 0b00001100; // PWM on
    T1CON = 0b10010000;     // Timer1 16 bits, prescaler = 01
    TMR1 = 0;
}

void duty_cycle(int var){
    CCPR1L = var << 2;
    CCP1CON = var >> 4;
    
    CCP1CON = CCP1CON | 0b000000110;
    CCP1CON = CCP1CON & 0b00111100;  
}


void main(){
    cfg();
    cfg_PWM();
    inicializa_LCD();
    
    int var = 0;
    float temp = 0;
    char c[16];
    
    comando(LINHA1);
    sprintf(c, "R.C: %d", var / 5);
    imprimir_dados_LCD(c);
    
    TMR1 = 0;
    
    while(1){
        T1CONbits.TMR1ON = 1;
        temp = TMR1;
        __delay_ms(1000);
        temp = TMR1;
        comando(LINHA2);
        sprintf(c, "%f", temp);
        imprimir_dados_LCD(c);
        // TMR1 = 2^16 - temp
        // Para que ocorra o estouro a cada 1 segundo
    }
}
