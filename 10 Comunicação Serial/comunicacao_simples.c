/* UART: Comunicacao assincrona, nao necessita de clock para sincronizar
 * usa apenas uma linha para a comunicacao
 * 
 * bit de start, inicia a transmissao, transicao de '1' para '0'
 * bit de paridade, opcional, detecta erros de transmissao
 *
 * Periodo do bit , tempo em que o bit permanece na linha de transmissao
 * Baud rate: Quantidade maxima de bits que pode ser transmitidia por segundo (bps)
 * 
 * TX = linhas de transmissao
 * RX = linhas de recepcao
 * 
 * Parametros de transmissao
 * -> Numero de bits:
 * -> Tamanho do stop bit:
 * -> Inclusao ou nao do bit de paridade:
 * -> Taxa de transmissao:
 *
 * Registrador TXSTA: Controla o modulo de transmissao EUSART
 * TXSTA = 0b76543210
 * 7 -> CSRC: selecao da origem de clock, comunicacao assincrona nao usa
 * 6 -> TX9: habilita a tranmissao do 9º bit) 1 = transmissao com 9 bits; 0 = tranmissao com 8 bits
 * 5 -> TXEN: habilita a transmissao) 1 = tranmissao habilitada; 0 = tranmissao nao habilitada
 * 4 -> SYNC: seleciona o modo de tranmissao) 1 = sincrona; 0 = assincrona
 * 3 -> SENDB: bit Send Caracter Break) 1 = Send Caracter Break na proxima tranmissao (apgado pelo hw apos o fim)
 * 2 -> BRGH: selecao do baud rate) na tranmissao assincrona: 1 = alta velocidade; 0 = baixa velocidade
 * 1 -> TRMT: status do registrador de deslocamento utilizado na transmissao (TSR)) 1 = TSR vazio; 0 = TSR cheio
 * 0 -> TX9D: nono bit de dados
 *
 * Registrador RCSTA, controle do modulo receptor
 * RCSTA = 0b76543210
 * 7 -> SPEN: habilita a porta serial) 1 = habilita; 0 = desabilita
 * 6 -> RX9: habilita recepcao do 9º bit) 1 = recepcao de 9 bits; 0 = recepcao de 8 bits
 * 5 -> SREN: habilita a recepcao unica) comunicacao assincrona nao possui funcao
 * 4 -> CREN: habilita recepcao continua) 1 = habilita; 0 = desabilita
 * 3 -> ADDEN: habilita deteccao de enderecos) 
 *												9 bits) 1 = habilita deteccao de enderecos; 0 = desabilita
 *												8 bits) Nao utilizado
 * 2 -> FERR: indica erro de frame) 1 = erro, stop bit lido como 0; 0 = nao houve erro
 * 1 -> OERR: erro de transbordo) 1 = houve transbordo; 0 = nao houve transbordo
 * 0 -> RX9D: nono bit
 *
 * Fluxograma
 * Setar o bit 5 de TXSTA, carregar o dado para o registrador TXREG -> dado carregado automaticamente para o registrador TSR
 * flag de interrupcao setada (TXIF)
 * 
 * -> Transmissor assincrono
 * 1) Inicialziar SPBRGH:SPBRGL com o baud rate desejado [SPBRG = 12: 9600 bauds por segundo]
 * 2) Configurar RC7:RC6 como entrada
 * 3) Habilitar a porta de comunicacao serial apagando SYNC e setando SPEN
 * 4) Habilitar a interrupcao se for desejado
 * 5) Habilitar o TXEN que setara tambem TXIF
 * 6) Carregar o dado no registrador TXREG
 * 7) Transmissao finalizada quando TRMT for setado pelo hardware
 * 
 * -> Receptor assincrono: Recebe os dados pelo pino RX, o dado e transmitido para o registrador RCREG, 
 * o bit RCIF e setado
 * 1) Inicializar o registrador SPBRG para o baud rate desejado
 * 2) Configurar RC7:RC6 como entrada
 * 3) Habilitar a porta serial apagando SYNC e setando SPEN
 * 4) Habilitar a interrupcao se desejado
 * 5) Habilitar a recepcao se desejado (CREN)
 * 6) RCIF e setado quando a recepcao estiver completa
 * 7) Ler os 8 bits de dados que se encontram em RCREG
 *
 *
 * RCIF = bit de sinalizacao da interrupcao do modulo receptor da porta serial
 * 1 = 8 bits chegaram a porta
 * 0 = nao chegou informacao pela porta
 *
 *
 * BAUD RATE = Fosc / (64 * (n + 1))
 *			   Fosc = Frequencia do oscilador
 *			   n = valor a ser atribuido a SPBRG
*/

// Exemplo de configuracao de transmissao serial assincrona
void configura_transm_serial(){
	TRISCbits.TRISC7 = 1;			// RC7 como entrada (RX)
	TRISCbits.TRISC6 = 1;			// RC6 como entrada (TX)
	
	SPBRG = 12;         			// 9600 bits por segundos
	
	// Registrador de controle do modulo transmissor do EUSART
	TXSTA = 0b00100000;				// <7> CSRC: origem do clock, nao utilizado em transmissao assincrona
									// <6> TX9: transmissao de 8 bits
									// <5> TXEN: tranmissao habilitada
									// <4> SYNC: tranmissao assincrona
									// <3> SENDB: tranmissao do send break completa
									// <2> BRGH: selecao do baud rate, em baixa velocidade
									// <1> TRMT: status do registrador de deslocamento, TSR cheio
	
	// Registrador de controle do modulo receptor
	RCSTA = 0b10010000;				// <7> SPEN: habilita a porta serial
									// <6> RX9: recepcao de 8 bits
									// <5> SREN: nao tem funcao na tranmissao assincrona
									// <4> CREN: habilita recepcao continua
									// <3> ADDEN: habilita deteccao de erro, nao habilitado
									// <2> FERR: indica erro de frame, 
									// <1> OERR: bit de erro de transbordo, nao houve transbordo
									// <0> RX9D: nono bit de dado
	
}

// ------- Configuracao PIC -------
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = ON

#define _XTAL_FREQ 8000000

#include <xc.h>
// ------- Configuracao PIC -------

#include <stdio.h>				// sprintf
#include <string.h>				// strlen

#define LED LATDbits.LATD0
char frase[10];

// ------- Enviar dados ----------
void envia_dado(char dado){
	PIR1bits.TXIF = 0;			// Seta a flag como 0
	RCREG = dado;				// RECG recebe o dado a ser enviado
	while(PIR1bits.TXIF == 0); 	// Espera para enviar o dado
}

void envia_frase(char frase[]){
	int tam = strlen(frase);
	for (int i = 0; i < tam; i ++){
		envia_dado(frase[i]);	// Envia dado a dado
		__delay_ms(1);			// Espera
	}
}
// ------- Enviar dados ----------

void config_PORTS(){
	TRISC = 0b10000000;			// RC7 como entrada, RX, recepcao
	TRISD = 0;					// LED
	TRISA = 0b00000100;			// Sensor em AN2, escolhido
}

void main(){
	config_PORTS();
	LATD = 0;
	
	ADCON0 = 0b00001001; 		// Seleciona o canal digital, controla a operacao do conversor
								// <7:6> Nao implementados
								// <5:2> CHS3:CHS0: 0010 -> AN2 selecionado
								// <1> GODONE = 0, desligado
								// <0> ADON: conversor habilitado
								
							
	ADCON1 = 0b00001100; 		// Configura os PORTS como digitais ou analogicas
								// <7:6> Nao implementados
								// <5> VFCG1: VREF-: VSS
								// <4> VFCG0: VREF+: VDD
								// <3:0> PCFG3:PCFG0: Configura as portas como A ou D
	
	ADCON2 = 0b10000000; 		// Configura a entrada
								// <7> ADFM: justificado a direita
								// <6> Nao implementados
								// <5:3> ACQT2:ACQT0: tmepo de aquisicao 000 = 0 TAD
								// <2:0> ADSC2?ADSC0: Selecao do clock 000 = Fosc/2
								
	// ------- Modulo EUART ---------
	TXSTA = 0b00100000;			// <5> TXEN: Transmissao habilitada
	RCSTA = 0b10010000;			// <7> SPEN: Porta serial habilitada
								// <4> CREN: Habilita recepcao continua
	// ------- Modulo EUART ---------
	
	SPBRG = 12;					// 12 = 9600 bauds/segundo
	
	int resultado_conv;
	
	while(1){
		var = 'a';
		
		if (var == 'a')
			LED = 1;
		if (var == 'b')
			LED = 0;
		if (var == 'x'){
			// Convesao AD
			ADCON0bits.GODONE = 1;			// Inicio a conversao AD
			while(ADCON0bits.GODONE == 1);	// Conversao acontecendo
			
			resultado_conv = (ADRESH << 8) + ADRESL;	// Valor recebido da conversao
			
			sprintf(frase, "%i\n\r", temp);
			
			envia_frase(frase);
 		}
		PIR1bits.RC1IF = 0; 				// Reseta a flag
	}
}

