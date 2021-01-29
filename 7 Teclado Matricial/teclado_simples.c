/*
 * File:   newmain.c
 * Author: Lucas
 * 
 * Imprimir o valor pressionado no dispaly LCD
 */

#include <pic18f4520.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON

#define _XTAL_FREQ 8000000

#include <xc.h>

#include <stdio.h>


#define LED LATDbits.LATD7
// *************************** LCD ***************************************************

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


// *************************** Teclado ***************************************************

#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
int varredura (){
    int valor = -1;

    LATD = 0b00001110;
    if (C1 == 0)
        valor = 1;
    if (C2 == 0)
        valor = 2;
    if (C3 == 0)
        valor = 3;
    
    LATD = 0b00001101;
    if (C1 == 0)
        valor = 4;
    if (C2 == 0)
        valor = 5;
    if (C3 == 0)
        valor = 6;
    
    LATD = 0b00001011;
    if (C1 == 0)
        valor = 7;
    if (C2 == 0)
        valor = 8;
    if (C3 == 0)
        valor = 9;
    
    LATD = 0b00000111;
    if (C1 == 0)
        valor = 10;
    if (C2 == 0)
        valor = 0;
    if (C3 == 0)
        valor = 11;
    
    return valor; 
}
// *************************** Teclado ***************************************************


void configurar_PORTS(){
    
    TRISE = 0;
    PORTE = 0;
    LATE = 0;
    
    TRISD = 0;
    LATD = 0;
    PORTD = 0;
    
    TRISB = 0b00000111;
    LATB = 0;
    PORTB = 0;         
    
    ADCON1 = 0b00001111;
    INTCON2bits.RBPU = 0;       // Liga os resistores de PULL UP
}


// Formata os valores para impressao
void formata (char txt[], int valor){
    if (valor < 10){
        sprintf(txt, "0%d", valor);
    }else{
        sprintf(txt, "%d", valor);
    }
}
void main(){
    configurar_PORTS();
    inicializa_LCD();
    
    comando(LINHA1);
    imprimir_dados_LCD("Contagem");
    
    int valor;
    char txt[4];
    while(1){
        valor = varredura();
        if (valor != -1){
            formata(txt, valor);
            comando(LINHA2);
            imprimir_dados_LCD(txt);
        }
    }
}