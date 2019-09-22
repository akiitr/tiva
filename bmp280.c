//---------------------------------------------------------------------------

// File: BMP280.c
// Title:       BMP280 interfacing with TM4C123GH6PM
// Description: Implements I2C using CMSIS.I2C module 1 is implemented.
//              The clock speed (SCL) is 100 KHz

//---------------------------------------------------------------------------
//MASTER: TIVA  TM4C123GH6PM
//SLAVE : BMP280 Pressure and temprature sensor

//---------------------------------------------------------------------------
//              Functions:
//For Initalization     : I2Cx_MasterOpen
//For Writing to Slave  : I2Cx_WriteData
//For Reading From Slave: I2Cx_ReadData

//---------------------------------------------------------------------------
//Author			:	Anubhav & Alkesh



#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

#include "tm4c123gh6pm.h"
#include "BMP_280_Driver.h"
#include "BMP_280_Driver.c"

#define Slave_ADDR      0x76 //BMP280 Slave address found from research
#define IODIRA_ADDR1    0x88 //BMP280 selction port values
#define IODIRA_ADDR2    0xA1 //BMP280 selction port values
#define GPIOA_ADDR      0x12 //GPIO Port A Address
#define GPIOB_ADDR      0x13 //GPIO Port B Address


//              Pinout:
//              SCL     SDA     Module
//              
//              PA6     PA7     I2C1
//---------------------------------------------------
//
//UART basic function pre declaration
//


void printChar3(char c);
void printChar2(char c);
void UART_OutUDec2(uint32_t n);
void UART_OutUDec3(uint32_t n);
//
//I2c basic funaction pre declaration
//
void I2C1_MasterOpen(void);
static void I2C1_BeginTransmission(uint8_t slaveAddress, uint8_t rs);
static uint8_t I2C1_WriteByte(uint8_t data, uint8_t start, uint8_t run, uint8_t stop);
uint8_t I2C1_WriteData(uint8_t slaveAddress, uint8_t regAddress, uint8_t data);
uint8_t I2C1_ReadData(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data);




int main(void)
{
    uint32_t i;
    uint8_t data1;
		uint8_t data2;
		
    
    I2C1_MasterOpen();  // Initializer I2C1 for master operation
    I2C1_WriteData(Slave_ADDR, IODIRA_ADDR1, 0x00);   // Configure IODIRA to all output
    
//For Reading Pressure from the BMP280 in data1 field
    
        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0xFF);   // Set GPIOA to all LOW
        for(i = 0; i < 400000; i++);
        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0x00);   // Set GPIOA to all HIGH
        for(i = 0; i < 400000; i++);
        
        // Read data from GPIOB
        BMP280ReadData(Slave_ADDR, GPIOB_ADDR, &data1);
        data1 = data1;

		I2C1_WriteData(Slave_ADDR, IODIRA_ADDR2, 0x00);	
//For Reading Temprature from the BMP280 in data2 field	

        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0xFF);   // Set GPIOA to all LOW
        for(i = 0; i < 400000; i++);
        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0x00);   // Set GPIOA to all HIGH
        for(i = 0; i < 400000; i++);
        
        // Read data from GPIOB
        I2C1_ReadData(Slave_ADDR, GPIOB_ADDR, &data2);
        data2 = data2;
        	
//UART configuration for sending transmission//
		//----------------------------------------------------------------------------

    // 1. Enabling uart 2 and 3  the UART module using the RCGCUART register 
    SYSCTL->RCGCUART |= (1<<2)|(1<<3); 
    
    // 2. Enable the clock to the C and D  GPIO module via the RCGCGPIO register 
    
		SYSCTL->RCGCGPIO |= (1<<2)|(1<<3); 
    
    // 3. Set the GPIO AFSEL bits for the appropriate pins 
    
		GPIOD->AFSEL = (1<<7)|(1<<6);
		GPIOC->AFSEL = (1<<7)|(1<<6);
		
    
    // 4. Configure the GPIO current level and/or slew rate as specified for the mode selected 
    
    // 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
    // pins 
    GPIOD->PCTL = (1<<28)|(1<<24);  
    GPIOC->PCTL = (1<<28)|(1<<24);  
    
		GPIOD->DEN = (1<<7)|(1<<6);
		GPIOC->DEN = (1<<7)|(1<<6);
    
    // Find  the Baud-Rate Divisor
    // BRD = 16,000,000 / (16 * 9600) = 104.16666666666666666666666666666666666666666666666666
    // UARTFBRD[DIVFRAC] = integer(0.166667 * 64 + 0.5) = 11
    
    
    // With the BRD values in hand, the UART configuration is written to the module in the following order
                   
    // 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register
    UART2->CTL &= ~(1<<0);
    UART3->CTL &= ~(1<<0);
    // 2. Write the integer portion of the BRD to the UARTIBRD register

		UART2->IBRD = 104;
    UART3->IBRD = 104;
		
    // 3. Write the fractional portion of the BRD to the UARTFBRD register.
    
		UART2->FBRD = 11; 
    UART3->FBRD = 11; 
    
    // 4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of 0x0000.0060)
    
		UART2->LCRH = (0x3<<5)|(1<<4);     // 8-bit, no parity, 1-stop bit
    UART3->LCRH = (0x3<<5)|(1<<4);     // 8-bit, no parity, 1-stop bit
    
    // 5. Configure the UART clock source by writing to the UARTCC register
		
    UART2->CC = 0x0;          
    UART3->CC = 0x0;          


    // 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
    
		UART2->CTL = (1<<0)|(1<<8)|(1<<9); 
    UART3->CTL = (1<<0)|(1<<8)|(1<<9); 
    

    
        UART_OutUDec2(data1);
				UART_OutUDec3(data2);
				
   
}


//-----------------------------------------------------------------
//UART function declaration for node mcu

//writing function for uart2
void printChar2(char c)  
{
    while((UART2->FR & (1<<5)) != 0);
    UART2->DR = c;           
}
//
void UART_OutUDec2(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART_OutUDec2(n/10);
    n = n%10;
  }
  printChar2(n+'0'); /* n is between 0 and 9 */
}

//

//writing function for uart3
void printChar3(char c)  
{
    while((UART3->FR & (1<<5)) != 0);
    UART3->DR = c;           
}

//
void UART_OutUDec3(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART_OutUDec3(n/10);
    n = n%10;
  }
  printChar3(n+'0'); /* n is between 0 and 9 */
}


//--------------------------------------------------------------
//I2C function declaration for bmp280 sensor
void I2C1_MasterOpen(void)
{
    // Enable I2C Module 1 Clock
    SYSCTL->RCGCI2C |= (1U << 1);
    while(!(SYSCTL->RCGCI2C & (1U << 1)));
    
    // Enable GPIO A Clock
    SYSCTL->RCGCGPIO |= (1U << 0);
    while(!(SYSCTL->RCGCGPIO & (1U << 0)));
    
    // Enable alternate function on PA6, PA7
    // Port Control set to I2C function
    // and digital enable PA6, PA7
    GPIOA->AFSEL    |= (1U << 6) | (1U << 7);
    GPIOA->PCTL     |= (3U << 24) | (3U << 28);
    GPIOA->DEN      |= (1U << 6) | (1U << 7);
    
    // SDA Line requires to be open collector
    GPIOA->ODR      |= (1U << 7);
    
    // I2C1 Master Function Enable
    I2C1->MCR = 0x00000010;
    
    // Set clock speed to 100kHz
    // System clock is 16 MHz
    // TPR  = System Clock / (( 2 * (SCL_LP + SCL_HP) * SCL_CLK ) - 1)
    //      = (16 000 000) / (( 2 * (6 + 4) * 100 000) - 1)
    //      = 7
		//for 80 mhz tpr value is 7*5=35 || 0x00000023
    I2C1->MTPR = 0x00000007;
    
}

// void I2C1_BeginTransmission(uint8_t slaveAddress, uint8_t rs)
// Set's the slave's address into Master Slave Address register
// uint8_t slaveAddress : address of slave
// uint8_t rs : receive or send bit.
//              0 = Transmit
//              1 = Receive
static void I2C1_BeginTransmission(uint8_t slaveAddress, uint8_t rs)
{

    I2C1->MSA = (slaveAddress << 1) + rs;   
}

// static uint8_t I2C1_WriteByte(uint8_t data, uint8_t start, uint8_t run, uint8_t stop)
// Sends a byte to the slave device.
// uint_8t data  : data to send to slave device. Can be actualy payload data or register address
// uint8_t start : 
// uint8_t run   :
// uint8_t stop  :
// return: uint8_t return value. 0 = no error, 1 = error error has occured
static uint8_t I2C1_WriteByte(uint8_t data, uint8_t start, uint8_t run, uint8_t stop)
{
    uint8_t error;
    I2C1->MDR = data;
    
    // If start bit sent, check if I2C Bus for busy bit
    // If busy, wait until it is busy
    if(start)
    {
        while(I2C1->MCS & 0x40);
    }
    
    I2C1->MCS = ( (run << 0) | (start << 1) | (stop << 2) );
    
    while(I2C1->MCS & 0x01);
    
    error = (I2C1->MCS & 0x02) >> 1;
    
    return error;
}

// uint8_t I2C1_WriteData(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
// uint8_t slaveAddress : address of the slave device
// uint8_t regAddress   : address of the register to write the data to
// uint8_t data         : data to write to the register
// return: uint8_t return value. 0 = no error. 1 = error while setting register address, 2 = error while writing data to register address
uint8_t I2C1_WriteData(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
{
    uint8_t error = 0;
    I2C1_BeginTransmission(slaveAddress, 0);        //  Set slave address to write to
    error = I2C1_WriteByte(regAddress, 1, 1, 0);    //  Set register address to write to
    if(error != 0)
    {
        error = 1;
        return error;
    }
    
    error = I2C1_WriteByte(data, 0, 1, 1);          //  Send data to specified register address of slave device 
        if(error != 0)
    {
        error = 2;
        return error;
    }
    
    return error;
}

// uint8_t I2C1_ReadData(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data)
// uint8_t slaveAddress : address of the slave device
// uint8_t regAddress   : address of the register to write the data to
// uint8_t *data        : pointer to where to store data to
// return: uint8_t return value. 0 = no error. 1 = error while setting register address, 2 = error while writing data to register address
uint8_t I2C1_ReadData(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data)
{
    uint8_t error = 0;
    I2C1_BeginTransmission(slaveAddress, 1);        //  Set slave address to read from
    error = I2C1_WriteByte(regAddress, 1, 1, 1);    //  Set register address to read from
    if(error != 0)
    {
        error = 1;
        return error;
    }
    *data = I2C1->MDR;
    return error;
}


