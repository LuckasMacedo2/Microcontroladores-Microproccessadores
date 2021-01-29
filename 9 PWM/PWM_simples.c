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
// ------- Configuracao PIC -------

void configura_PWM(){
	CCP1CON = 0b00001100;	// <3:0> ->  CCP1M3:CCP1M0 = 11xx -> Operacao PWM
	PR2 = 124;
	T2CON = 0b00000000;
}
