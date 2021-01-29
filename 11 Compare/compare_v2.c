/*
 * File:   lcd.c
 * Author: Lucas
 *
 * Created on 13 de Abril de 2019, 13:45
 * 
 * Codigo: Contador de 15 a 0 com 0.5 segundos de intervalo
 */
#include <string.h>
#include <stdio.h>      // sprintf
#define TAM_NUM 3       // Tamanho do vator de char que armazena o numero como char

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

#include <xc.h>
#include <pic18f4520.h>

#define _XTAL_FREQ 8000000

#define LCD PORTD        // LCD
#define CONTROLE_LCD     // Controle do LCD

// --- LCD ---
// Definindo os pinos do LCD
#define EN LATEbits.LATE1   // Pino enable, inicia o ciclo de leitura/escrita
#define RS LATEbits.LATE2   // Pino que define se e dado ou instrucao  
//#define RW LATEbits.LATE0   // Pino de leitura ou escrita

// Comando
void comando(int cmd){
    RS = 0;                 // Sera enviada uma instrucao ao Display, RS = 0 instrucao
    EN = 1;                 // Inicia o ciclo
    LATD = cmd;             // Enviando para D (DB0 a DB7) o comando
    __delay_ms(2);          // Delay necessario para manter os dados
    EN = 0;                 // Termina o ciclo, compreende um pulso
}

// Limpa o display
void limpa_LCD(){
    // Limpa o display e retorna o cursor para a primeia posicao
    comando(1);             // Aplica 1 em DB0
}

// Inicializa o LCD
void iniciliza_LCD (){
    EN = 0;                 // Envia instrucao
    RS = 0;                 // Limpa o pino enable            
    //RW = 0;                 // Ativa o ciclo de leitura
    
    comando(0b00111100);    // Inicializa o display
    comando(0b00001100);    // Liga o lcd
    limpa_LCD();            // Limpa o lcd
}

// Exibe um caracter na tela do LCD
void enviar_dados_LCD(int dado){
    RS = 1;                 // Define que sera enviado um dado ao LCD
    EN = 1;                 // Inicializa o ciclo de leitura/escrita do LCD
    LATD = dado;            // Envia o dado as linhas de dados (DB0 a DB7/ RD0 a RD7)
    __delay_ms(20);         // Delay necessario para manter os dados
    EN = 0;                 // Finaliza o ciclo de leitura e escrita, pulso
}

void imprimir_dados_LCD(char frase[]){
    int i = 0;              // Indice do vetor frase
    while (frase[i] != 0){  // 0 indica o fim do vetor
        enviar_dados_LCD(frase[i]);
        i++;
    }
}

// --- Configuracao ---
void config (){  
    // Botoes
    TRISB = 0;
    TRISC = 0b00000100;     // Botao em RC2
    PORTB = 0;
    TRISA = 0b11111111;
            
    // LCD
    TRISD = 0;              // Controle do LCD
    TRISE = 0;              // Controle do LCD
      
    // PORTS
    ADCON1 = 15;            // Ports como digitais
}

void conf_PWM(int var){
    CCPR1L = var >> 2;
    CCP1CON = var << 4;
    CCP1CON = CCP1CON | 0b00001100;
    CCP1CON = CCP1CON & 0b00111100;
}


float tempo1 = 0, tempo2 = 0;
 
char valor[16];

int x = 0;                  // Eventos

void imprimir(){
        //limpa_LCD();

    sprintf(valor, "%f", tempo2 - tempo1);

    // Imprimir o valor na LCD
    comando(192);
    imprimir_dados_LCD(valor);
}

// Referencia: https://www.electronicwings.com/pic/pic18f4550-timer-capture


void main(void) {
    config();               // Inicializa PORTS, buttons, interrupcoes, etc
    
    // RC2 timmer1
    T1CON = 0b00010001;    
    CCP1CON = 0b00000101;   // 0101 -> Modo de captura em borda de subida
    
    iniciliza_LCD();        // Inicializa o LCD
    
    comando(128);
    
    imprimir_dados_LCD("CAPTURE");
    
    TRISCbits.RC2 = 1;
    
   // PIE1bits.CCP1IE=1;
   // PIR1bits.CCP1IF=0;
    
    while(1){
        // Primeiro evento
        while (PIR1bits.CCP1IF == 0);
        PIR1bits.CCP1IF = 0;
        tempo1 = (CCPR1H << 8) + CCPR1L;
        LATB = 8;
        
        // Segundo evento        
        while (PIR1bits.CCP1IF == 0);
        PIR1bits.CCP1IF = 0;
        tempo2 = (CCPR1H << 8) + CCPR1L;
        LATB = 16;
        
        imprimir();
        
       /* TRISCbits.RC2 = 1;  
        while (PIR1bits.CCP1IF == 0){
            if (x == 0){
                tempo1 = (CCPR1H << 8) + CCPR1L; // Armazena o valor  
                x = 1;                           // Tempo inicial
            }else{
                tempo2 = (CCPR1H << 8) + CCPR1L;
                x = 0;                           // Tempo finaal
                imprimir();
            }
            PIR1bits.CCP1IF = 0;                 // Resetando a flag
           TRISCbits.RC2 = 0;  
        }*/
    } 
}