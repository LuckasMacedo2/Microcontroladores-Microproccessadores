// Configuracao
#pragma config LVP = OFF
#pragma config MCLRE = ON
#pragma config WDT = OFF
#pragma config OSC = HS

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <pic18f4520.h>
// ---

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

// -- Conversao AD --
void cfg_conv_AD(){
    ADCON0 = 0b00000001; //Esolhe as portas; AN0 e conversor ligado
    ADCON1 = 0b00001110; // AN0 analogica; VREF+ = VDD, VREF- = VSS
    ADCON2 = 0b10101001; // Justificado a direita, TAD = 12 e Fosc/8
}
float conversao_AD(){
    float valor_conversao;

    ADCON0bits.GODONE = 1;  // inicia a conversao;
        
    while (ADCON0bits.GODONE == 1); // Espera conversao
        
    valor_conversao  = (ADRESH << 8) + ADRESL;   // ADRESL 8 bits mais significativos
                                                // ADRESL bits menos significativo
    // (ADRESH << 8) + ADRESL == ADRES, mesmoo valor
    valor_conversao = (valor_conversao/1023)*5;
    
    return valor_conversao;
}
// -- Conversao AD --


#define COOLER LATCbits.LATC2

void cfg(){
    // Controle do barramento de dados do LCD
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
    
    // Controle do LCD
    PORTE = 0;
    LATE = 0;
    TRISE = 0;
    
    // Conversor AD
    TRISA = 1;
    PORTA = 0;
    LATA = 0;
    
    // Cooler
    TRISC = 0;
    PORTC = 0;
    LATC = 0;
}

void main(){
    cfg();
    inicializa_LCD();
    
    cfg_conv_AD();  // Conversao AD
    
    
    comando(LINHA1);
    enviar_frase("Conversor AD");
    
    char temp[16];
    float resultado = 0;
    
    while (1) {
        resultado  = conversao_AD();
        
        // Printa no LCD
        sprintf(temp, "%f", resultado);
        comando(LINHA2);
        enviar_frase(temp);
        
        // Liga o cooler se temperatura > 3
        if (resultado > 3)
            COOLER = 1;
        else
            COOLER = 0;
    }

}