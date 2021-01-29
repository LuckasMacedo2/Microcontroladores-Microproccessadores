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
 Timer2: Só funciona como temporizador de 8 bits, estouro quando o valor se iguala a PR2
 * Postscaler seta a flag de interrupcao de Timer2
 * Quando configurada como contador o Timer1 pode ser contador sincrono ou assincrono
 * 
 * T1CON = 0b76543210
 * 7 -> -
 * 6:3 -> T2OUTPS3:T2OUTPS0) Selecao do postscaler
 * 2 -> TMR2ON) Liga o modulo Timer2: 1 = Liga; 0 = Desliga
 * 1:0 -> Selecao do prescaler
 *          00 = 1:1
 *          01 = 1:4
 *          1x = 1:16 
 * 
 */ 

// Exemplo de configuracao TMR1
void configura_TMR2(){      // bit
    T2CON = 0b00011110;     // <6:3> Postscaler de 1:4
                            // <1:0> Prescaler de 1:16
                            // <2> Liga o Timer2
    // Numeros de incremento antes do estouro
    // N_inc = 256 - valor_inicial
    // Tempo de estouro
    // TE = N_inc * (4/Fosc) * prescaler
}