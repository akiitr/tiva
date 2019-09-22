 #define SYSCTL_RCGC2_R (*((volatile unsigned long*)0x400FE108))
 //data register for port A 
 #define GPIO_PORTA_DATA_R (*((volatile unsigned long*)0x400043FC))
 //data register for port F 
 #define GPIO_PORTF_DATA_R (*((volatile unsigned long*)0x400253FC)) 
 #define GPIO_PORTA_DIR_R (*((volatile unsigned long*)0x40004400))
 #define GPIO_PORTF_DIR_R (*((volatile unsigned long*)0x40025400))
 #define GPIO_PORTA_AFSEL_R (*((volatile unsigned long*)0x40004420))
 #define GPIO_PORTF_AFSEL_R (*((volatile unsigned long*)0x40025420))
 #define GPIO_PORTA_PCTL_R (*((volatile unsigned long*)0x4000452C))
 #define GPIO_PORTF_PCTL_R (*((volatile unsigned long*)0x4002552C))
 #define GPIO_PORTA_LOCK_R (*((volatile unsigned long*)0x40004520))
 #define GPIO_PORTF_LOCK_R (*((volatile unsigned long*)0x40025520))
 #define GPIO_PORTA_CR_R (*((volatile unsigned long*)0x40004524))
 #define GPIO_PORTF_CR_R (*((volatile unsigned long*)0x40025524))
 #define GPIO_PORTA_DEN_R (*((volatile unsigned long*)0x4000451C))
 #define GPIO_PORTF_DEN_R (*((volatile unsigned long*)0x4002551C))
 #define GPIO_PORTA_AMSEL_R (*((volatile unsigned long*)0x40004528))
 #define GPIO_PORTF_AMSEL_R (*((volatile unsigned long*)0x40025528))
 #define GPIO_PORTA_PUR_R (*((volatile unsigned long*)0x40004510))
 #define GPIO_PORTF_PUR_R (*((volatile unsigned long*)0x40025510))
 #define GPIO_PORTA_PDR_R (*((volatile unsigned long*)0x40004514))
 #define GPIO_PORTF_PDR_R (*((volatile unsigned long*)0x40025514))
// //to be used for output Of AND fn
// #define GPIO_PORTA_PA0 (*((volatile unsigned long*)0x40004004))
// //to be used for output Of OR fn 
// #define GPIO_PORTA_PA1 (*((volatile unsigned long*)0x40004008))
// //to be used for output Of XOR fn 
// #define GPIO_PORTA_PA2 (*((volatile unsigned long*)0x40004010)) 
// //to be used for input as a switch
// #define GPIO_PORTF_PF0 (*((volatile unsigned long*)0x40025004))
// //to be used for input as a switch 
// #define GPIO_PORTF_PF4 (*((volatile unsigned long*)0x40025040))

 // Defining And function for input pin PF0 nad PF4 and output at PA0

 void And_Gate()
 {
 GPIO_PORTA_DATA_R =((GPIO_PORTF_DATA_R>>4) & GPIO_PORTF_DATA_R)<<2;
 }

 // Defining Or function for input pin PF0 nad PF4 and output at PA1

 void Or_Gate()
 {
 GPIO_PORTA_DATA_R |=((GPIO_PORTF_DATA_R>>4) | GPIO_PORTF_DATA_R)<<3;
 }

 // Defining Xor function for input pin PF0 nad PF4 and output at PA2

 void Xor_Gate()
 {
 GPIO_PORTA_DATA_R |=((GPIO_PORTF_DATA_R>>4) ^ GPIO_PORTF_DATA_R)<<4;
 }


 int main()

 {

 SYSCTL_RCGC2_R |=0x00000021; //Activate clock for PORT A and PORT F
 GPIO_PORTF_DIR_R=0x00; //PORTF Pin 0,4 set as input
 GPIO_PORTA_DIR_R |=0x1C; //PORTA Pin 0,1,2 set as output
 GPIO_PORTA_AFSEL_R=0x00; //Choosing function
 GPIO_PORTF_AFSEL_R=0x00; //Choosing function
 GPIO_PORTA_LOCK_R=0x4C4F434B; //unlock the PORTA register
 GPIO_PORTF_LOCK_R=0x4C4F434B; //unlock the PORTF register
 GPIO_PORTA_CR_R |=0x1C; //Authorizing the change to be considered
 GPIO_PORTF_CR_R |=0x11; //Authorizing the change to be considered
 GPIO_PORTA_DEN_R |=0x1C; //Enabling The digital Port A
 GPIO_PORTF_DEN_R |=0x11; //Enabling The digital Port F
 GPIO_PORTA_AMSEL_R=0x00;//Disabling The digital Port A
 GPIO_PORTF_AMSEL_R=0x00;//Disabling The digital Port F
 GPIO_PORTF_PUR_R=0x11; //due to negative logic of input pins
GPIO_PORTF_DATA_R=0x00; 


 while(1) // for continuous loop excecution
 {

	//GPIO_PORTA_DATA_R=0xFF;
 And_Gate(); //Calling And Function within loop
 Or_Gate();//Calling OR Function within loop
 Xor_Gate();//Calling XOR Function within loop

 }
 
  return 0;
  }