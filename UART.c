#include "stdint.h"
#include "stdbool.h"
#include "tm4c123gh6pm.h"
///////////////////////////////////////////////////////////////////////////
//Defines

//our baud rate is 9600, so to get Clock divisor:
//  16MHZ/(16*9600) = 104.166666666667
//So IntBaud is 104
#define IntBaud 104
// fractionBaud :
// round((16667*64)+0.5) = 11
#define FractionBaud 11
/////////////////////////////////////

//funcition defs
void UART0_Init(void);

char UART0_GetChar(void);
void UART0_SendChar(char c);

void printString(char * string);
void UART0_Init(void)
{
    SYSCTL_RCGCUART_R |= 0x01;//enable clock for uart0
    SYSCTL_RCGCGPIO_R |=0x01; //enable clock for port A
    GPIO_PORTA_AFSEL_R |= 0x03; //enable alternate func. for portA0-1
    GPIO_PORTA_AMSEL_R &= ~(0x03); //disable analog for portA0-1
    GPIO_PORTA_DEN_R |= 0x03; //digital enable PortA0-1
    GPIO_PORTA_PCTL_R =  (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;//enable UART0 PA0 as a reciever, PA1 as a transmitter

    UART0_CTL_R &= ~(0x01); //disable UART0

    //Configure baud rate
    UART0_IBRD_R = IntBaud;
    UART0_FBRD_R = FractionBaud;

    UART0_LCRH_R = 0x70; //8-bit word length,enable FIFO
    UART0_CC_R= 0x00; //use default clock source
    UART0_CTL_R = 0x0301; //enable RX,TX,UART

    }
char UART0_GetChar(void)
{
    char c;
    while((UART0_FR_R&0X10)!=0);
        //Wait until recieve fifo is not empty
    c=UART0_DR_R;
        return c ;
    }
 
void UART0_SendChar(char c)
{
    while((UART0_FR_R&0X20)!=0);
    //Wait until send fifo is not full
    UART0_DR_R = c;

    }

void printString(char * string)
{
    while(*string)
    {
        UART0_SendChar(*(string++));
    }
    }



/////////////////////// TASKA SENDING & RECIEVING FROM PC
/*
   int main(void)
{
    UART0_Init();
    InitLEDs();
    char c;
    printString("Enter r, g, or b:\n\r");

    while(1)
    {
       c= UART0_GetChar();
       UART0_SendChar(c);
       printString("\n\r");

       switch(c)
       {
       case 'r':
           GPIO_PORTF_DATA_R^=0X02U;
           break;
       case 'g':
                GPIO_PORTF_DATA_R^=0X08U;
                break;
       case 'b':
                GPIO_PORTF_DATA_R^=0X04U;
                break;
       default:
           break;
       }
    }

    return 0;
}


*/

