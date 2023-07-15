#include <msp430.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

#define UART_PINS BIT5 | BIT4
#define MAPPER_UNLOCK_KEY 0X02D52
#define TRANSMISSION_PIN BIT4
#define RECEPTION_PIN BIT5
const int SMCLK_FREQ = UCSSEL_2;

void initializePins(void){
    P4DIR |=  TRANSMISSION_PIN;   //P4.4 output
    P4DIR &= ~RECEPTION_PIN;      //P4.5 input
    P4SEL |= UART_PINS;           // Select UART functionality for pins
}

void remapPins(void){
    PMAPKEYID = MAPPER_UNLOCK_KEY;    //Unlock pin mapping
    P4MAP4 = PM_UCA1TXD;              //TX=Sair por P4.4
    P4MAP5 = PM_UCA1RXD;              //RX=Receber por P4.5
}

void disableUartReset(void){
    UCA1CTL1 &= ~ UCSWRST;  //UCSI sai do Reset
}

void uartDefaultConfiguration(void){
    #define defaultValue 0
    UCA1CTL0 = defaultValue;// 1 bit stop, no parity, 8bit message (char)
}

void setBoudrate(void){
    UCA1BRW = 109;//Set baud rate to 9600
}

// USCI_A1: Configurar Serial 1 (MSP <=> PC)
// P4.4 = TX (MSP ==> PC)
// P4.5 = RX (MSP <== PC)
void uartSetup(void){
    UCA1CTL1 =  SMCLK_FREQ |   //SMCLK=1.048.576Hz
                UCSWRST;       //USCI_A1 in reset state
    uartDefaultConfiguration();
    setBoudrate();
    UCA1MCTL=UCBRS_2|UCBRF_0;  //UCOS16=0

    initializePins();
    remapPins();

    disableUartReset();
}

void transmitChar(char character){
    UCA1TXBUF = character; // Load character into the UART transmit buffer
    __delay_cycles(1000);
}

void newLine(void){
    transmitChar('\r'); // Transmit carriage return
    transmitChar('\n'); // Transmit line feed
    transmitChar('\r'); // Transmit carriage return
}

void waitForReadyToTransmit(void){
    while ( (UCA1IFG&UCTXIFG)==0);  // Wait until TXIFG flag is set
}

void printUart(char texto[]){
    int count;
    int size = strlen(texto);
    for (count = 0; count < size; count++){
        waitForReadyToTransmit();
        transmitChar(texto[count]);
        if (texto[count + 1] == 0){
            newLine();
        }
    }
}
