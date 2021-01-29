/*
 * File:   newmain.c
 * Author: Lucas
 *
 * Ficar contando de 0 a 100, ao pressionar RB0 (Alta prioridade) ligar a ventoinha e contar ate zero
 * Ao pressionar RB2 (baixa prioridade) resetar a contagem
 */

#include<stdio.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000

#include <xc.h>

/*
 * Display LCD: cristal liquido e um estado intermediario da materia entre o solido e o liquido
 * Pinos do Display LCD
 * <1> -> VSS: Terra
 * <2> -> VDD: 5V
 * <3> -> V0: Ajuste de contraste
 * <4> -> RS: Indica se e dado ou instrucao) 1 = Dado; 0 = instrucao.
 * <5> -> R/W: Indica se e leitura/escrita) 1 = Leitura; 0 = Escrita
 * <6> -> E: Inicia o ciclo de leitura/escrita) Aplicar nivel logico '0' depois de algum tempo o nivel logico 1
 * <7:14> -> DB0:DB7: Pinos de dados) Permite leitura e escrita, DB7 busy flag, indica quando o processamento da instrucao termina
 * <15> -> A: Anodo
 * <16> -> K: Catodo
 * 
 * 192 = Primeira linha
 * 128 = Segunda linha
 * Limpar display = 1
 */ 

#define LINHA1 128
#define LINHA2 192

// *************************** LCD ***************************************************
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

int valor = 0;
#define VENTOINHA LATCbits.LATC2

void reset_LCD(){
    limpar_LCD();
    comando(LINHA1);
    imprimir_dados_LCD("Contagem");
    comando(LINHA2);
}

void inicializa(){
    // Barramento de dados DB7: DB0
    TRISD = 0;      
    PORTD = 0;
    LATD = 0;
    
    // Pinos E (Enable) e pino RS (dado ou instrucao)
    TRISE = 0;
    PORTE = 0;
    LATE = 0;
    
    // Ventoinha
    TRISC = 0;
    LATC = 0;
    PORTC = 0;
    
    //Botoes
    TRISB = 0b000000101;
    PORTB = 0;
    LATB = 0;
}

void conta(){
    reset_LCD();
    char txt[4];
    for (; valor < 1000; valor ++){
        comando(LINHA2);
        sprintf(txt, "%d", valor);
        imprimir_dados_LCD(txt);
        __delay_ms(100);
    }
    valor = 0;
}

void conta_ate_0(){
    reset_LCD();
    char txt[4];
    for (; valor >= 0; valor --){
        comando(LINHA2);
        sprintf(txt, "%d", valor);
        imprimir_dados_LCD(txt);
        __delay_ms(100);
    }
    valor = 0;
}



// Interrupcoes
void conf_interrupcoes(){
    // Interrupcao geral
    RCONbits.IPEN = 1;              // Habilita os niveis prioridade das interrupcoess
    INTCONbits.GIEH = 1;            // Habilita as interrupcoes de alta prioridade
    INTCONbits.GIEL = 1;            // Habilita as interrupcoes de baixa prioridade
    
    // RB0    
    INTCONbits.INT0IE = 1;          // Habilita a interrupcao em RB0
    INTCONbits.INT0IF = 0;          // Seta o flag de interrupcao como 0
    INTCON2bits.INTEDG0 = 0;        // Borda de descida
    
    // RB2
    INTCON3bits.INT2IE = 1;         // Habilita as interrupcoes em RB2
    INTCON3bits.INT2IF = 0;         // Seta o flag de interrupcao como 0
    INTCON2bits.INTEDG2 = 0;        // Borda de descida
    
    INTCON3bits.INT2IP = 0;         // RB2 como baixa prioridade
}

void __interrupt(high_priority) isr_alta(){
    if (INTCONbits.INT0IF == 1){
        VENTOINHA = 1;
        conta_ate_0();
        VENTOINHA = 0;
    }
    INTCONbits.INT0IF = 0;
}

void __interrupt(low_priority) isr_low(){
    if (INTCON3bits.INT2IF == 1){
        valor = 0;
    }
    INTCON3bits.INT2IF = 0;
}



void main(){
    inicializa();
    inicializa_LCD();
    conf_interrupcoes();
        
    reset_LCD();
    
    while (1){
        conta();
    }
}
