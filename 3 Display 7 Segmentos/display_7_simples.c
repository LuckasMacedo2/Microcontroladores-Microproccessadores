/*
 * File:   newmain.c
 * Author: Lucas MS
 *
 * Created on 1 de Junho de 2019, 11:38
 * 
 * Contar de 0 a F em cada display de 7 segmentos do kit
 */

/* No kit
 * os pinos estao ligados da seguinte forma:
 * RD0 -> segmento a 
 * RD1 -> segmento b
 *        ...
 * RD6 -> segmento g
 * RD7 -> segmento ponto 
 * 
 * Para escolher qual display esta acesso
 * RA2 -> Display 1
 * RA3 -> Display 2
 * RA4 -> Display 3
 * RA5 -> Display 4
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#define _XTAL_FREQ 8000000
#include <xc.h>

#define DISP1 LATAbits.LATA2    // Display 1
#define DISP2 LATAbits.LATA3    // Display 2
#define DISP3 LATAbits.LATA4    // Display 3
#define DISP4 LATAbits.LATA5    // Display 4

void inicializa()
{
    ADCON1 = 0b00001111;        // Todas as portas ANn como digitais
    
    // Controle dos displays
    PORTA = 0;
    LATA = 0;
    TRISA = 0;
    
    // Controle dos segmentos do display de 7 segmentos
    PORTD = 0;
    LATD = 0;
    TRISD = 0;
}

const int int_to_led(int index  ){
    const int v[16] = {
        63,     // 0
        6,      // 1
        91,     // 2
        79,     // 3
        102,    // 4
        109,    // 5
        125,    // 6
        7,      // 7
        127,    // 8
        111,    // 9
        119,    // A 
        124,    // B
        57,     // C
        94,     // D
        121,    // E
        113     // F
    };
    
    return v[index];
}

void contagem_0_F(){
    for (int i = 0; i < 16; i++){
        LATD = int_to_led(i);
        __delay_ms(500);
        LATD = 0;
    }
}

void main(void) {
    inicializa();
    while(1){
        // Display 1
        DISP1 = 1;
        contagem_0_F();
        DISP1 = 0;
        
        // Display 2
        DISP2 = 1;
        contagem_0_F();
        DISP2 = 0;
        
        // Display 3
        DISP3 = 1;
        contagem_0_F();
        DISP3 = 0;
        
        // Display 4
        DISP4 = 1;
        contagem_0_F();
        DISP4 = 0;
    }
}
