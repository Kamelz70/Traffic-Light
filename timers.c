#include <inc\tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>


void  TIMER0_Init();
void  TIMER1_Init();
void  TIMER0_Delay(unsigned int delay);
//////////////////////////////////initializers
void  TIMER0_Init()
{
  SYSCTL_RCGCTIMER_R|=0X01; //ENABLE CLOCK FOR T0
  TIMER0_CTL_R=0X0; //DISABLE BEFORE INITIALIZATION
  TIMER0_CFG_R=0X00; //USE 32-BIT TIMER 
  TIMER0_TAMR_R=0X01;//ONE-SHOT DOWN COUNTER
  TIMER0_TAPR_R=0;
  TIMER0_ICR_R=0X1; //CLEAR TIMEOUT FLAG
  TIMER0_IMR_R=0X01; //ENABLE INTERRUPT
  NVIC_PRI4_R=(NVIC_PRI4_R&0X00FFFFFF)|0X80000000;//SET PRORITY OF 4 TO TIMER0
  NVIC_EN0_R=(1<<19);//ENABLE IRQ 19 IN NVIC
}
void TIMER1_Init()
{
   SYSCTL_RCGCTIMER_R|=(1<<1); //ENABLE CLOCK FOR T0
  TIMER1_CTL_R=0X0; //DISABLE BEFORE INITIALIZATION
  TIMER1_CFG_R=0X00; //USE 32-BIT TIMER 
  TIMER1_TAMR_R=0X01;//ONE-SHOT DOWN COUNTER
  TIMER1_ICR_R=0X1; //CLEAR TIMEOUT FLAG
  TIMER1_IMR_R=0X01; //ENABLE INTERRUPT
  NVIC_PRI5_R=(NVIC_PRI5_R&0XFFFF0FFF)|0X04000;//SET PRORITY OF 2 TO TIMER1
  NVIC_EN0_R=(1<<21);//ENABLE IRQ 21 IN NVIC

}
/////////////////////////////////////////////////////////Delay set
void TIMER0_Delay(unsigned int delay0)
{
//delay is in milliseconds
    TIMER0_TAILR_R= (unsigned int)(delay0*16000)-1;    //LOAD VALUE  
    TIMER0_CTL_R|=0X3;//ENABLE TIMER0 AND ENABLE STALLING
     //MAKE SURE THAT THE INTERRUPT IS FROM T0 NOT FROM PORTF SWITCH AND SLEEP UNTILL IT IS ELAPSED
  
      while( TIMER0_TAILR_R!=0)
      {
   __asm("     wfi\n");
   if( TIMER0_TAILR_R!=0)
   {
   RestoreState();
   }
      }
    
}
void TIMER1_Delay(unsigned int delay1)
{
//delay is in milliseconds
    TIMER1_TAILR_R= (unsigned int)(delay1*16000)-1;    //LOAD VALUE
    TIMER1_CTL_R|=0X3;//ENABLE TIMER1 AND ENABLE STALLING
      
   __asm("     wfi\n");
    
}
/////////////////////////////////////////////exception handlers
void Timer0Handler(void)
{
  
  TIMER0_CTL_R=0X00; //DISABLE TIMER
 TIMER0_TAILR_R= 0X0; //SET LOAD VALUE TO 0
  
TIMER0_ICR_R = (0X01);//CLEAR RAW INTERRUPT STATUS timeout flag
}


void Timer1Handler(void)
{
  
  TIMER1_CTL_R=0X00; //DISABLE TIMER
// TIMER1_TAILR_R= 0X0; //SET LOAD VALUE TO 0

TIMER1_ICR_R = (0X01);//CLEAR RAW INTERRUPT STATUS timeout flag
}