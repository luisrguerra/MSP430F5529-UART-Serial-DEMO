#include <msp430.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

// USCI_A1: Configurar Serial 1 (MSP <=> PC)
// P4.4 = TX (MSP ==> PC)
// P4.5 = RX (MSP <== PC)
void uartSetup(void){
    UCA1CTL1 =  UCSSEL_2 |     //SMCLK=1.048.576Hz
                UCSWRST;       //UCSI em Reset
    UCA1CTL0 = 0;              //1 Stop
    UCA1BRW = 109;             //
    UCA1MCTL=UCBRS_2|UCBRF_0;  //UCOS16=0

    P4DIR |=  BIT4;         //P4.4 sa�da
    P4DIR &= ~BIT5;         //P4.5 entrada
    P4SEL |= BIT5 | BIT4;   //Selecionar UART

    PMAPKEYID = 0X02D52;    //Liberar mapeamentp
    P4MAP4 = PM_UCA1TXD;    //TX=Sair por P4.4
    P4MAP5 = PM_UCA1RXD;    //RX=Receber por P4.5

    UCA1CTL1 &= ~ UCSWRST;  //UCSI sai de Reset
}


void printUart(char texto[]){
    int count;
    for (count = 0; count < strlen(texto); count++){
        while ( (UCA1IFG&UCTXIFG)==0);  //Wait TXIFG=1
        UCA1TXBUF=texto[count];
        __delay_cycles(1000);
        if (texto[count + 1] == 0){
            UCA1TXBUF='\r';
            __delay_cycles(1000);
            UCA1TXBUF='\n';
            __delay_cycles(1000);
            UCA1TXBUF='\r';
        }
    }
}
