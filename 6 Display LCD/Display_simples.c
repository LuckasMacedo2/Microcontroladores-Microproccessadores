/*
 * File:   newmain.c
 * Author: Lucas
 *
 * Escrever uma mensagem no display LCD
 */

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

void inicializa(){
    // Barramento de dados DB7: DB0
    TRISD = 0;      
    PORTD = 0;
    LATD = 0;
    
    // Pinos E (Enable) e pino RS (dado ou instrucao)
    TRISE = 0;
    PORTE = 0;
    LATE = 0;
}

void main(){
    inicializa();
    inicializa_LCD();
    
    comando(LINHA1);
    imprimir_dados_LCD("Display 16x2");
    comando(LINHA2);
    imprimir_dados_LCD("Lucas Macedo!");
    
    while (1);
}
