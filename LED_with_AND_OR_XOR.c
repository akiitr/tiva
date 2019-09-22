
#define SYSCTL_RCGC2_R     (*((volatile unsigned long*)0x400FE108))	          // clock to port A AND F
#define GPIO_PORTF_DATA_R  (*((volatile unsigned long*)0x400253FC))           // GPIO PORTF DATA REGISTER
#define GPIO_PORTA_DATA_R  (*((volatile unsigned long*)0X400043FC))           // GPIO PORTA DATA REGISTER
#define GPIO_PORTF_DIR_R   (*((volatile unsigned long*)0x40025400))           // GPIO PORTF direction register
#define GPIO_PORTA_DIR_R   (*((volatile unsigned long*)0X40004400))           // GPIO PORTA DIRECTION REGISTER
#define GPIO_PORTF_AFSEL_R (*((volatile unsigned long*)0x40025420))           // GPIO PORTF AFSEL REGISTER
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long*)0x40004420))           // GPIO PORTA AFSEL REGISTER
#define GPIO_PORTF_LOCK_R  (*((volatile unsigned long*)0x40004520))           // GPIO PORTF LOCK REGISTER
#define GPIO_PORTA_LOCK_R  (*((volatile unsigned long*)0x40025520))           // GPIO PORTA AFSEL REGISTER
#define GPIO_PORTF_CR_R    (*((volatile unsigned long*)0x40025524))           // GPIO PORTF CR REGISTER
#define GPIO_PORTA_CR_R    (*((volatile unsigned long*)0x40004524))           // GPIO PORTA CR REGISTER
#define GPIO_PORTF_DEN_R   (*((volatile unsigned long*)0x4002551C))           // GPIO PORTF DIGITAL ENABLE
#define GPIO_PORTA_DEN_R   (*((volatile unsigned long*)0x4000451C))           // GPIO PORTA DIGITAL ENABLE
#define GPIO_PORTF_AMSEL_R (*((volatile unsigned long*)0x40025528))           // GPIO PORTF ANALOG MODE SELECT
#define GPIO_PORTA_AMSEL_R (*((volatile unsigned long*)0X40004528))           // GPIO PORTA ANALOG MODE SELECT
#define GPIO_PORTF_PCTL_R  (*((volatile unsigned long*)0x4002552C))           // GPIO PORTF PCTL REGISTER
#define GPIO_PORTA_PCTL_R  (*((volatile unsigned long*)0x4000452C))           // GPIO PORTA PCTL REGISTER
#define GPIO_PORTF_PUR_R   (*((volatile unsigned long*)0x40025510))           // GPIO PORTF PULL UP REGISTER
#define GPIO_PORTA_PUR_R   (*((volatile unsigned long*)0x40004510))           // GPIO PORTA PULL UP REGISTER

int main(void){ 
	SYSCTL_RCGC2_R     |= 0x00000021;      // PORTA AND PORTF  CLOCK
   GPIO_PORTF_LOCK_R  =0x4C4F434B;       // UNLOCK PORTF 
   GPIO_PORTA_LOCK_R  =0x4C4F434B;       // UNLOCK PORTA 
   GPIO_PORTF_CR_R    |=0x11;             // ALLOW CHANGE TO PORTF PF0
   GPIO_PORTA_CR_R    |=0x1C;             // ALLOW CHANGE TO PORTA
   GPIO_PORTF_DIR_R=  0x00;             // PF4,PF0 input 
   GPIO_PORTA_DIR_R |=  0x1C;             // PF0,PF1,PF2 OUTPUT
   GPIO_PORTA_AFSEL_R =0x00;             // no alternate function at portA
   GPIO_PORTF_AFSEL_R = 0x00;            // NO ALTERNATE FUNCTION AT PORTF
   GPIO_PORTA_AMSEL_R =0x00;             // DISABLE ANALOG FUNCTION AT PORTA
   GPIO_PORTF_AMSEL_R = 0x00;            // DISABLE ANALOG FUNCTION AT PORTF
   GPIO_PORTA_DEN_R   |=0x1C;             // ENABLE DIGITAL PINS PF0, PF1,PF2 OF PORTA
   GPIO_PORTF_DEN_R   |= 0x11;            // ENABLE DIGITAL PINS PF0,PF4 OF PORTF
	GPIO_PORTF_PUR_R=0x11;
	GPIO_PORTF_DATA_R=0x00;
 
  while(1){
		
	 GPIO_PORTA_DATA_R =  ((GPIO_PORTF_DATA_R>>4) & (GPIO_PORTF_DATA_R))<<2;        // AND OPERATION OF TWO INPUTS,
	 GPIO_PORTA_DATA_R |= ((GPIO_PORTF_DATA_R>>4) | (GPIO_PORTF_DATA_R))<<3;        // OR OPERATION OF TWO INPUTS
	 GPIO_PORTA_DATA_R |=((GPIO_PORTF_DATA_R>>4) ^ (GPIO_PORTF_DATA_R))<<4;        // EXOR OPERATION OF TWO INPUTS
	 }
 }    
 


  