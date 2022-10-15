#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <inc\gpio.h>
#include <inc\bitwise_operation.h>
void Init_PortF(void)
{
  SYSCTL_RCGCGPIO_R |= 0x00000020;
  while((SYSCTL_PRGPIO_R&0x00000020)==0);
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0x1F;
  GPIO_PORTF_DIR_R = 0x0E;
  GPIO_PORTF_PUR_R = 0x11;
  GPIO_PORTF_DEN_R = 0x1F;

    GPIO_PORTF_IS_R &= ~(1<<4)|~(1<<0); //MAKE SWITCHES INTERRUPTS' EDGE SENSETIVE
  GPIO_PORTF_IBE_R &= ~(1<<4)|~(1<<0);// trigger controlled by iev
    GPIO_PORTF_IEV_R &= ~(1<<4)|~(1<<0); //MAKE SWITCHES INTERRUPTS' NEGATIVE EDGE TRIGGERED
  NVIC_PRI7_R=(NVIC_PRI7_R&0XFF0FFFFF)|0X00600000;//SET PRIORITY OF PORTF INTERRUPT TO 2
  GPIO_PORTF_ICR_R |= (1<<4)|(1<<0);
  GPIO_PORTF_IM_R=(1<<4)|(1<<0);//ENABLE INTERRUPTS FOR BOTH PORTF SWITCHES
  NVIC_EN0_R=(1<<30);//ENABLE INTERRUPTS FOR PORTF
}

void Init_PortB(void)
{
  SYSCTL_RCGCGPIO_R |= (1<<1);
  
  while((SYSCTL_PRGPIO_R&0x00000002)==0);
  GPIO_PORTB_CR_R = 0x9F;
  GPIO_PORTB_DIR_R = 0x9F;
  GPIO_PORTB_PUR_R = 0x00;
  GPIO_PORTB_DEN_R = 0x9F;
}

void Init_PortC(void)
{
  SYSCTL_RCGCGPIO_R |= (1<<2);
  
  while((SYSCTL_PRGPIO_R&0x00000004)==0);
  GPIO_PORTC_CR_R = 0xF0;
  GPIO_PORTC_DIR_R = 0xF0;
  GPIO_PORTC_PUR_R = 0x00;
  GPIO_PORTC_DEN_R = 0xF0;
}


void WritePin(int Register, int Bit,int set)
{
  if (set ==1)
  {
  Set_Bit(Register,Bit);
  }
  if(set == 0)
  {
    Clear_Bit(Register,Bit);
  }
}
int ReadPin(int Register,int Bit)
{
  return Get_Bit(Register,Bit);
}
///////////////////HANDLERS
//////////I NEED TO ENABLE PORTF INTERRUPTS & FROM NVIC & PRIORITY
//THEN ADD HANDLER HERE AND IN EWARM STARTUP ALSO --Done

///////////clear interrupt after finishing
void PortFHandler(void)
{
      TIMER0_CTL_R=0X0; //DISABLE TRAFFIC TIMER

  ////////handle if a padestrian pressed a button
   if (GPIO_PORTF_MIS_R&0x10) //check if interrupt caused by PF4(SW1)=E-W
    {   
      
    printString("E-W Pedestrian passing\n\r");
       //1 NSR ,2 NSY ,3 NSG ,4 EWR ,5 EWY ,6 EWG ,7 PNSR ,8 PNSG ,9 PEWR ,10 PEWG
    TrafficLights(0,0,1,1,0,0,1,0,0,1);
    TIMER1_Delay(2000);
      GPIO_PORTF_ICR_R |= 0x10; /* clear the interrupt flag */
     } 
    else if (GPIO_PORTF_MIS_R& 0x01) //check if interrupt caused by PF0(SW2)=N-S
    {   
      printString("N-S Pedestrian passing\n\r");
    TrafficLights(1,0,0,0,0,1,0,1,1,0);
    TIMER1_Delay(2000);
    
     
     GPIO_PORTF_ICR_R |= 0x01; /* clear the interrupt flag */
    }
    
    TIMER0_CTL_R|=0X3;////ENABLE TRAFFIC TIMER AGAIN
}


