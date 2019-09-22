
//13Aug2018
//Meghnath and Tejendra

#include<stdio.h>
#include "tm4c123gh6pm.txt"


#define SW1 (GPIO_PORTF_DATA_R&(1<<4))
#define SW2 (GPIO_PORTF_DATA_R&(1<<0))
#define LED_AND (GPIO_PORTF_DATA_R=0X02) 
#define LED_OR (GPIO_PORTF_DATA_R=0X04)
#define LED_EXOR (GPIO_PORTF_DATA_R=0X08)




int main()
{
		SYSCTL_RCGC2_R |=0x00000020;     //enable port F and port E
//	for(int i=0;i<10;i++)
//	{
//	}
	
	GPIO_PORTF_LOCK_R=0x4C4F434B;      //enble PF0 pin by giving lock value in Lock Register
	GPIO_PORTF_CR_R=0x01F;             // Provide Read and write access to PF0-PF4 and PD0-PD4 pin
	GPIO_PORTF_AMSEL_R=0x00;           //Disable Analog mode on selected pins	
  GPIO_PORTF_PCTL_R=0x00000000;  
	GPIO_PORTF_DIR_R=0x0E;            //set Directions of PF0-PF4 and PD0-PD4 pin
	GPIO_PORTF_AFSEL_R=0x00;          //Altrnate function disable
	GPIO_PORTF_PUR_R=0x11;            // Make Pull up reresistor for PF0 and PF4
	GPIO_PORTF_DEN_R=0x1F;            //Make Selected pin as Digital pin  

	
	while(1)
	{
	
	if( (SW1 ) && (SW2) )
{
		
	LED_AND;   // AND Operation
		
	}
	else if( (SW1) || (SW2)  )
	{
	LED_OR;  //OR operation
	}
	else if( ((SW1) && (!SW2))  ||( (!SW1) && (SW2))  )	
	{
	
	LED_EXOR;   //  ExOR Operation
	}
	
	}
return 0;
}
	



