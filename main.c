#include <inc\tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
//FUNCTION DEFS
void TrafficLights(bool NSR,bool NSY,bool NSG,bool EWR,bool EWY,bool EWG,bool PNSR,bool PNSG,bool PEWR,bool PEWG);
void RestoreState(void);
enum state {  NSGREEN,      ////////// N-S GREEN LIGHT ON
              NSYELLOW ,     ////////// N-S YELLOW LIGHT ON
              STOEW ,    ////////// BOTH LIGHTS OFF FOR 1 SEC TO SWITCH TO E-W
              EWGREEN ,    ////////// E-W GREEN LIGHT ON
              EWYELLOW ,    ////////// E-W YELLOW LIGHT ON
              STONS  ////////// BOTH LIGHTS OFF FOR 1 SEC TO SWITCH TO N-S
              }; 

volatile enum state TrafficState = NSGREEN;

int main()
{
  //initialize ports
   Init_PortF();
   Init_PortB();
   Init_PortC();
     //INITIALIZE TIMERS
  TIMER0_Init(); 
  TIMER1_Init();
  //initialize UART
  UART0_Init();

//ENABLE INTERRUPTS
__asm("CPSIE I");
while(1)
{
  switch(TrafficState)
  {
   //1 NSR ,2 NSY ,3 NSG ,4 EWR ,5 EWY ,6 EWG ,7 PNSR ,8 PNSG ,9 PEWR ,10 PEWG
    case NSGREEN:
    //SEND STRING TO PC STATING CURRENT STATE
    printString("N-S GREEN ON\n\r");
    TrafficLights(0,0,1,1,0,0,1,0,0,1);
    //delay in ms
    TIMER0_Delay(5000);

    TrafficState=NSYELLOW;
    break;
/////////////////////////////////////////////////

    case NSYELLOW:
    printString("N-S YELLOW ON\n\r");
    TrafficLights(0,1,0,1,0,0,1,0,0,1);
    TIMER0_Delay(2000);

    TrafficState=STOEW;

    break;
/////////////////////////////////////////////////

    case STOEW:
    printString("SWITCHING FROM N-S TO E-W (1 SEC RED DELAY)\n\r");

    TrafficLights(1,0,0,1,0,0,1,0,1,0);
    TIMER0_Delay(1000);

    TrafficState=EWGREEN;
    break;
/////////////////////////////////////////////////
    case EWGREEN:
    printString("E-W GREEN ON\n\r");

    TrafficLights(1,0,0,0,0,1,0,1,1,0);

    TIMER0_Delay(5000);
    TrafficState=EWYELLOW;
    break;
/////////////////////////////////////////////////
    case EWYELLOW:
    printString("E-W YELLOW ON\n\r");
    
    TrafficLights(1,0,0,0,1,0,0,1,1,0);
    
    TIMER0_Delay(2000);
    TrafficState=STONS;

    break;
/////////////////////////////////////////////////

    case STONS:
    printString("SWITCHING FROM E-W TO N-S (1 SEC RED DELAY)\n\r");

    TrafficLights(1,0,0,1,0,0,1,0,1,0);
    TIMER0_Delay(1000);

    TrafficState=NSGREEN;
    break;
    
  }
}
  return 0;


}


void RestoreState(void)
{ 
switch(TrafficState)
  {
    case NSGREEN:
    //SEND STRING TO PC STATING CURRENT STATE
    printString("N-S GREEN ON\n\r");
    TrafficLights(0,0,1,1,0,0,1,0,0,1);
    break;
/////////////////////////////////////////////////

    case NSYELLOW:
    printString("N-S YELLOW ON\n\r");
    TrafficLights(0,1,0,1,0,0,1,0,0,1);
    break;
/////////////////////////////////////////////////

    case STOEW:
    printString("SWITCHING FROM N-S TO E-W (1 SEC RED DELAY)\n\r");
    TrafficLights(1,0,0,1,0,0,1,0,1,0);;
    break;
/////////////////////////////////////////////////
    case EWGREEN:
    printString("E-W GREEN ON\n\r");
    TrafficLights(1,0,0,0,0,1,0,1,1,0);
    break;
/////////////////////////////////////////////////
    case EWYELLOW:
    printString("E-W YELLOW ON\n\r"); 
    TrafficLights(1,0,0,0,1,0,0,1,1,0);
    break;
/////////////////////////////////////////////////

    case STONS:
    printString("SWITCHING FROM E-W TO N-S (1 SEC RED DELAY)\n\r");
    TrafficLights(1,0,0,1,0,0,1,0,1,0);
    break;
    
  }

}


//FUNCTION TO SET ALL LIGHTS, NSR= (N-S RED), EWR = (E-W RED), PNSR=(PEDESTRIAN N-S RED), EWSR = (PEDESTRIAN E-W RED)
void TrafficLights(bool NSR,bool NSY,bool NSG,bool EWR,bool EWY,bool EWG,bool PNSR,bool PNSG,bool PEWR,bool PEWG)
{
  ///DISABLE INTERRUPTS UNTILL SWITCHING LIGHTS FINISHES
__asm("CPSID I");

  //SET EACH TRAFFIC LIGHT TO ITS CORRESPONDING BIT IN PORTB
  GPIO_PORTB_DATA_R=(NSR<<0)|(NSY<<1)|(NSG<<2)|(EWR<<3)|(EWY<<4)|(EWG<<7);
  //10011111
  //SET EACH PEDESTRIAN LIGHT TO ITS CORRESPONDING BIT IN PORTC
  GPIO_PORTC_DATA_R=(PNSR<<4)|(PNSG<<5)|(PEWR<<6)|(PEWG<<7);

  ///ENABLE INTERRUPTS BACK ON
__asm("CPSIE I");

}



/////////////enable portf interrupts to use its' swithes