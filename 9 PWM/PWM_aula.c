/* UART: Comunicacao assincrona, nao necessita de clock para sincronizar
 * usa apenas uma linha para a comunicacao
 * 
 * OS modulos CCP1 e CCP2 geram os sinais PWM
 * CCPxCON ativa o modo PWM
 * Aplica 11xx em CCPxM3:CCPxM0 ativa a operacao PWM
 *
 * O ciclo PWM se inicia quando TMR2 == PR2
 * E finaliza quando TMR2 == (CCPR1L:CCP1CON<5:4>)
 * 
 * CCP1RL:CCP1CON: Alteram o tamanho do ciclo ativo
 * 
 * O sinal PWM e gerado pelo pino RC2/CCP1
 * Periodo e frequencia sao definidas pelo valor em PR2, pela Fosc e pelo fator prescaler aplicado em TMR2
 * 				 T = (PR2 + 1) * 4 * Tosc * (Prescaler)
 *				 Tosc = 1 / Fosc
 *				
 *				 PR2 = Fosc/(Fpwm * 4 * Prescaler) - 1
 *				 
 *				 PWM ciclo ativo = (CCPR1L:CCP1CON<5:4>) * Tosc * Prescaler
 *
 *	O tempo correspondente ao ciclo ativo e controlado por 10 bits (CCPR1L bits mais significativos e CCP1X:CCP1Y bits menos significativos)
 *
 * CCP1CON = 0b76543210
 * 7 -> Nao implementado
 * 6 -> |||
 * 5 -> DC1B1: Usado pelo PWM armazena os bits menos significativos
 * 4 -> DCB0: 					|||
 * 3 -> CCP1M3: Usado para definir o modo de operacao
 * 2 -> CCP1M2: |||
 * 1 -> CCP1M1: ||| 
 * 0 -> CCP1M0: |||
 *
 * CCP1M3:CCP1M0 = 11xx -> Operacao PWM
 */
// ------- Configuracao PIC -------
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <pic18f4520.h>
// ------- Configuracao PIC -------

#include <stdio.h>
#include <pic18.h> // sprintf

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

void conf_PWM(int var){
    CCPR1L = var>>2;		// Obtem os 6 bits mais significativos
    CCP1CON = var<<4;
    CCP1CON = CCP1CON | 0b00001100;
    CCP1CON = CCP1CON & 0b00111100;
}

void main(){
    inicializa();
    inicializa_LCD();
    
    CCP1CON = 0b00001100;	// <3:0> ->  CCP1M3:CCP1M0 = 11xx -> Operacao PWM
	
    T2CON = 0b00000110;     // <7> -> Nao implementado
                            // <6:3> T2OUTPS3:T2OUTPS0: Selecao do postscaler  
                            // <2> TMR2ON: Liga o Timer2
                            // <1:0> T2CKPS1:T2CKPS0: Selecao do prescaler 1x = 1:16
    
    PR2 = 124;              // Inicia o ciclo quando TMR2 == PR2
    
    int var = 50;
    
    char c[10];
    while (1){
        comando(LINHA2);
        sprintf(c, "%d/100", (var*50)/100);
        limpar_LCD();
        imprimir_dados_LCD(c);
        __delay_ms(4000);
        conf_PWM(var);
        
        var += 50;
        if (var == 500){
            var =  0;
        }
    }
}
