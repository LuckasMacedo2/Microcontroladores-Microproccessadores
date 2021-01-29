// Configuracao
// CONFIG1H
#pragma config OSC = RCIO6      // Oscillator Selection bits (External RC oscillator, port function on RA6)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <pic18f4520.h>

// -- LCD --
#include <stdio.h>

#define LINHA1 128
#define LINHA2 192

#define RS LATEbits.LATE2
#define E LATEbits.LATE1

void comando(int cmd){
    E = 1;      // Ativa o ciclo
    RS = 0;     // Instrucao
    
    LATD = cmd;
    __delay_ms(3);
    
    E = 0;      // Finaliza o ciclo
}


void limpar_LCD(){
    comando(1);
}


void inicializa_LCD(){
    RS = 0;
    E = 0;      // Termina o ciclo
    comando(0b00111100);        // Inicializa o LCD
    comando(0b00001100);        // Liga o lcd
    limpar_LCD();               // Limpa o LCD
}

void enviar_dados(int cmd){
    E = 1;      // Ativa o ciclo
    RS = 1;     // 1 = Dado
    
    LATD = cmd;
    __delay_ms(20);
    
    E = 0;      // Termina o ciclo
}

void enviar_frase(char frase[]){
    int i = 0;
    while (frase[i] != 0){
        enviar_dados(frase[i]);
        i ++;
    }  
}
// -- LCD --

// -- PWM --
void config_PWM(){
    CCP1CON = 0b00001100; // Define modo de operacao PWM
    T2CON = 0b00000110; // Prescaler de 16, Timer2 ligado
    
    PR2 = 124;          // PR2, ciclo quando TMR2 == PR2
}

void duty_cycle(int var){
    // CCPR1L = 0baaaaaaaa
    CCPR1L = var >> 2;  // Desloca dois bits para a direita, obtem os 6 bits mais significativos
    //CCP1CON = 0bccddcccc - > CCP1CON = 0bddcccccc
    CCP1CON = var << 4;
    // Mantendo a configuracao original
    CCP1CON = CCP1CON | 0b00001100;
    CCP1CON = CCP1CON & 0b00111100;
}

// Link: https://luizbitencourt.wordpress.com/2016/10/13/usando-o-modulo-pwm-do-pic-com-c18/
// -----------------------------------------------------
// Rotina de configuração do Periodo do PWM
// -----------------------------------------------------
// Para cálculo do período do PWM (p. 153 do datasheet):
// Periodo = [(PR2) + 1] * 4 * Tosc * (Timer2 Prescaler)
// -----------------------------------------------------
void Periodo_PWM(unsigned char Valor){
   T2CON = 0x04;         // Timer2 desligado, Prescaler = 1 (p. 137 do datasheet)
   PR2 = Valor;          // Registrador que contém o período do Timer2 (p. 137 do datasheet)
}
// -----------------------------------------------------

// -----------------------------------------------------
// Rotina de configuração de Duty Cycle do PWM
// -----------------------------------------------------
// Para cálculo do Duty Cycle do PWM (p.154 do datasheet):
// DutyCycle = (CCPR1L:CCP1CON) * TOSC * (TMR2 Prescaler)
// -----------------------------------------------------
void DutyCycle_PWM(unsigned int Valor){
   CCP1CONbits.DC1B0 = Valor;
   CCP1CONbits.DC1B1 = Valor >> 1;
   CCPR1L = Valor >> 2;
}
// -----------------------------------------------------
// -- PWM --

/*
 Desenvolva
um programa para o microcontrolador utilizando o IDE
MPLAB X (compilador XC 8 que ajuste o módulo CCP para a função
PWM com uma frequência de chaveamento de 1 kHz
O
programa deve alterar a razão cíclica do chaveamento do motor de
0 a 100 de 10 em 10 Cada razão cíclica deve permanecer
por 4 segundos Os valores de razão cíclica PWM, em termos
percentuais, devem ser apresentados na linha superior do LCD
 
 Periodo = [(PR2) + 1] * 4 * Tosc * (Timer2 Prescaler)
 * f = 1KHz
 * Tpwm = 1ms
 * PR2 = 124 (chute)
 * 1ms = 125 * 4 * 1/(8*10^6) * Prescaler
 * Prescaler = 1ms/(125 * 4 * 1/(8*10^6))
 * Prescaler = 16
 * 
 * 1ms = perido
 * 1ms - 100%
 * x - n
 * x = 1ms*n/100% 
 */

void conf_REGS(){
    // Dados LCD
    PORTD = 0;
    LATD = 0;
    TRISD =  0;
    
    // Controle LCD
    PORTE = 0;
    LATE = 0;
    TRISE = 0;
    
    // Saida
    PORTC = 0;
    LATC = 0;
    TRISC = 0;
}

void main(){
    conf_REGS();
    inicializa_LCD();
    config_PWM();
    
    int var = 50;
    char valor[16];
    
    // LCD
    comando(LINHA1);
    enviar_frase("PWM Duty Cycle");
    comando(LINHA2);

    while (1){
        comando(LINHA2);
        sprintf(valor, "%i/100", var / 5);
        enviar_frase(valor);
        __delay_ms(400);
        
        duty_cycle(var);
        
        var += 50;
        if (var == 500)
            var = 0;
    }
}

