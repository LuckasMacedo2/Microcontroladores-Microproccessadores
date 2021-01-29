/*
 * File:   newmain.c
 * Author: Lucas
 * 
 * Medir a tensao de forma que ao atingir um valor maior que 3 volts ligue o cooler ao ficar menor que
 * 3 volts o cooler deve ser desligado
 */

/*
 * Conversao A/D
 * 
 * Tensao de referencia: Define a faixa de tensao que o conversor A/D pode receber em sua entrada
 * VREF+: Define o valor maximo de tensao
 * VREF-: Define o valor minimo de tensao
 * VREF+ - VREF-: Define a faixa de tensao a ser convertida
 * 
 * Possui resolucao maxima de 10 bits
 *  
 * ADCON0 -> Controla a operacao do conversor
 * ADCON1 -> Configura a funcao dos pinos de entrada
 * ADCON2 -> Configura a origem do clock
 * 
 * ADCON0 = 0b76543210
 * 5:2 ->  CHS3:CHS0: Selecao do canal analogico
 * 1 -> GO/DONE: Inicia a conversao A/D) 1 = conversao em progresso
 *                                       0 = fim da conversao setado por hw
 * 0 -> ADON: Halibita o conversor) 1 = conversor em operacao; 0 = conversor desligado
 * 
 * ADCON1 = 0b76543210
 * 5 -> VCF1: Selecao da tensao VREF- ) 1 = AN2; 0 = VSS
 * 4 -> VCF0: Selecao da tensao VREF+ ) 1 = AN3; 0 = VDD
 * 3 -> PCFG3:PCFG0: Configura os pinos do conversor A/D
 *      1111 = Todo mundo digital
 * 
 * ADCON2 = 0b76543210
 * 7 -> ADFM: Justificacao) 1 = justificado a direita; 0 = justificado a esquerda
 * 5:3 -> ACQT2:ACQT0: Tempo de aquisicao)
 *     111 = 20TAD      011 = 6TAD
 *     110 = 16TAD      010 = 4TAD
 *     101 = 12TAD      001 = 2TAD
 *     100 = 8TAD       000 = 0TAD
 * 2:0 -> Selecao do clock do conversor
 *  111 = FRC
 *  110 = Fosc/64
 *  101 = Fosc/16 
 *  100 = Fosc/4     
 *  011 = FRC
 *  010 = Fosc/32
 *  001 = Fosc/8
 *  000 = Fosc/2 
 * 
 * 
 * Fluxograma conversao
 * 1) Configurar os pinos como entrada, registrador TRISx
 * 2) Configurar os pinos como entradas analogicas, tensao de referencia e os pinos digitais (ADCON1)
 * 3) Selecionar o canal em que a tensao nele eaplicada sera convertida (ADCON0)
 * 4) Selecionar o clock (ADCON2), selecionar a justificacao (ADCON2)
 * 5) Ligar o conversor A/D
 * 6) Dar inicio a conversao, bit GO/DONE (ADCON0)
 * 7) Esperar GO/DONE ser apagado pelo hw
 * 8) Ler o resultado da conversao em ADRESH:ADRESL
 * 
 * 
 * Resolucao do conversor A/D
 *              R = Vfs/2^n; n == 10, numero de bits e Vfs = tensao fundo de escala
 *  */
#include <stdio.h>

#include <pic18f4520.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON
#define _XTAL_FREQ 8000000


#include <xc.h>


int configura_conv_AD(){
    // Selecionando o canal
    ADCON0 = 0b00000001;        // <5:2> CHS3:CHS0: 0000 = canal 0 (AN0)    
                                // <1> GO/DONE: Conversao desligada 
                                // <0> ADON: Conversor ligado
    // Configurando os pinos
    ADCON1 = 0b00001110;        // <5> VCFG1: VREF- em (VSS)
                                // <4> VCFG0: VREF+ em (VDD)
                                // <3:0> PCFG3:PCFG0: 1110 = AN0 como analogico AN1:AN12 como digitais
    // Origem do clock, aquisicao e justificacao
    ADCON2 = 0b10101001;        // <7> ADFM: justificado a direita
                                // <5:3> ACQT2:ACQT0: tempo de aquisicao = 101 = 12TAD
                                // <2:0> ADSC2:ADSC0: selecao do clock = 001 = Fosc/8   
}       

float conversao_AD(){
    float r = 0;
    ADCON0bits.GODONE = 1;                      // Iniciar a conversao
    while(ADCON0bits.GODONE == 1);              // Espera o fim da conversao
    r = (ADRESH<<8)+(ADRESL);                   // Obtem a conversao
    return r;
}

void configurar_PORTS(){
    // Cooler
    TRISC = 0;
    PORTC = 0;
    LATC = 0;
    
    
    TRISA = 1;              // AN0;
    PORTA = 0;
    LATA = 0;
   
    // Led
    TRISD = 0;
    LATD = 0;
    PORTD = 0;
}

#define COOLER LATCbits.LATC2
#define LED LATDbits.LATD0

void main(){
    configurar_PORTS();
    configura_conv_AD();
    
    float valor;
    while(1){
        valor = conversao_AD();
        valor = valor / 1024;
        valor = valor * 5;
        
        if (valor >= 3){
            COOLER = 1;
            for (int i = 0; i < valor; i++){
                LED = 1;
                __delay_ms(10);
                LED = 0;
                __delay_ms(10);
            }
        }
        else
            COOLER = 0;
    }
}