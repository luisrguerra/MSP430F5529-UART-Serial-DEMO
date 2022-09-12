#include <msp430.h>
#include <uart.h>

// Mudar o terminal para serial terminal, baude rate 9600,
// definir o serial port de acordo com o que aparece no gereciador
// de dispositivos em portas (No caso foi COM7)

char msg[] = "Hello World";

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    uartSetup();
    while(1){
      printUart(msg);
    }
    return 0;
}
