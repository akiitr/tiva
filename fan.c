//programm for the assigned problem:--
//-------------------------------------------
/*
Objective Of the Programm:-

Design a temperature controller to switch on a fan when the temperature exceeds
30oC using TIVA TM4C123G board where digital GPIO PC4 acts as input port and digital
GPIO PD3 acts as output port. Explain the circuit diagram. The maximum voltage to input
port is 3.3V and minimum voltage to input port is 0V.*/

//Defining the name for the frequently used register for easy to use in programm
// name are self explanatory for the registor name

//defining system clock for different port
#define SYSCTL_RCGC2_R (*((volatile unsigned long*)0x400FE108))
//data register for port C  
#define GPIO_PORTC_DATA_R (*((volatile unsigned long*)0x40006EFC))
//data register for port D 
#define GPIO_PORTD_DATA_R (*((volatile unsigned long*)0x40007EFC)) 
#define GPIO_PORTC_DIR_R (*((volatile unsigned long*)0x40006400))
#define GPIO_PORTD_DIR_R (*((volatile unsigned long*)0x40007400))
#define GPIO_PORTC_AFSEL_R (*((volatile unsigned long*)0x40006420))
#define GPIO_PORTD_AFSEL_R (*((volatile unsigned long*)0x40007420))
#define GPIO_PORTC_PCTL_R (*((volatile unsigned long*)0x4000652C))
#define GPIO_PORTD_PCTL_R (*((volatile unsigned long*)0x4000752C))
#define GPIO_PORTC_LOCK_R (*((volatile unsigned long*)0x40006520))
#define GPIO_PORTD_LOCK_R (*((volatile unsigned long*)0x40007520))
#define GPIO_PORTC_CR_R (*((volatile unsigned long*)0x40006524))
#define GPIO_PORTD_CR_R (*((volatile unsigned long*)0x40007524))
#define GPIO_PORTC_DEN_R (*((volatile unsigned long*)0x4000651C))
#define GPIO_PORTD_DEN_R (*((volatile unsigned long*)0x4000751C))
#define GPIO_PORTC_AMSEL_R (*((volatile unsigned long*)0x40006528))
#define GPIO_PORTD_AMSEL_R (*((volatile unsigned long*)0x40007528))
#define GPIO_PORTC_PUR_R (*((volatile unsigned long*)0x40006510))
#define GPIO_PORTD_PUR_R (*((volatile unsigned long*)0x40007510))
#define GPIO_PORTC_PDR_R (*((volatile unsigned long*)0x40006514))
#define GPIO_PORTD_PDR_R (*((volatile unsigned long*)0x40007514))

	int main()

{
	SYSCTL_RCGC2_R |=0x0000000C; //Activate clock for PORT C and PORT D
	GPIO_PORTC_DIR_R=0x00; //PORTC Pin C4 set as input
	GPIO_PORTD_DIR_R |=0x08; //PORTA Pin D3 set as output
	GPIO_PORTC_AFSEL_R=0x00; //Choosing function
	GPIO_PORTD_AFSEL_R=0x00; //Choosing function
	GPIO_PORTC_LOCK_R=0x4C4F434B;  //unlock the PORTC  register
	GPIO_PORTD_LOCK_R=0x4C4F434B;  //unlock the PORTD  register
	GPIO_PORTC_CR_R |=0x10;        //Authorizing the change to be considered
	GPIO_PORTD_CR_R |=0x08;        //Authorizing the change to be considered
	GPIO_PORTC_DEN_R |=0x10;        //Enabling The digital Port C
	GPIO_PORTD_DEN_R |=0x08;        //Enabling The digital Port D
	GPIO_PORTC_AMSEL_R=0x00;//Disabling The digital Port C
	GPIO_PORTD_AMSEL_R=0x00;//Disabling The digital Port D
	//GPIO_PORTA_PUR_R=0x00;
	//GPIO_PORTF_PUR_R=0x11; //due to negative logic of input pins
	

	
	while(1) // for continuous loop excecution
	{
		//Assigning the output to PD3 port
		GPIO_PORTD_DATA_R= (GPIO_PORTC_DATA_R<<1);
		
	}
	
return 0;	
}