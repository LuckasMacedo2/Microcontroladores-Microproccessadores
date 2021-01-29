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

// -- Conversao AD --
void cfg_conv_AD(){
    ADCON0 = 0b00000001; //Esolhe as portas; AN0 e conversor ligado
    ADCON1 = 0b00001101; //AN1 analogica; VREF+ = VDD, VREF- = VSS
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


void cfg(){
    // LCD
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
   
    
    // Conversor AD
    TRISA = 1;
    PORTA = 0;
    LATA = 0;
   
    
    // LED
    PORTB = 0;
    LATB = 0;
    TRISB = 0;
}

void main(){
    
    /*
    Desenvolva um programa que efetue a
    leitura do canal AN1 e acione os LEDs
    do PORTD conforme o valor da tensão
    lida. Desta forma, os LEDs devem ser
    acionados gradativamente conforme a
    leitura da tensão, sendo que 0V mantém
    todos os LEDs desligados e 5V mantém
    todos os LEDs ligados.
     */ // USEI AN0
    
    cfg();    
    cfg_conv_AD();  // Conversao AD
    
    float resultado = 0;
    
    while (1) {
        resultado  = conversao_AD();
        
        if (resultado == 0){
            LATD = 0;
        }else if  (resultado <= 1.5){
            LATD = 0b00000011;
        }else if (resultado <=2.5){
            LATD = 0b00001111;
        }else if (resultado <=3.5){
            LATD = 0b00111111;
        }else{
            LATD = 0b11111111;
        }
    }

}