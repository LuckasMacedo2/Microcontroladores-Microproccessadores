/*
 * File:   ex9.c
 * Author: Lucas
 *
 * Created on 5 de Março de 2019, 17:56
 */


#include "config.h"
#include <math.h>

void inicializa(){
    PORTB = 0b00000000;
    PORTC = 0b00000010;
    PORTD = 0b00000000;
    
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    return;
}


void main(void) {
    inicializa();
    int cont = 1;
    
    int on_off = 1;
    
    while (1){   

        for (int i = 0; i <= 5; i ++){
            PORTD = 128*on_off + pow (2,i);
            __delay_ms(500);
            if (cont == 4){
                if (on_off == 1){
                    on_off = 0;
                }else{
                    on_off = 1;
                }
                cont = 0;
            }
            cont ++;
        }
    
    }
    return; 
}
