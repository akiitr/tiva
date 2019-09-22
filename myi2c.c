
#include<stdio.h> 

#include "tm4c123gh6pm.h"

#define Slave_ADDR   0x76
#define IODIRA_ADDR     0x00
#define GPIOA_ADDR      0x12
#define GPIOB_ADDR      0x13

//              Pinout:
//              SCL     SDA     Module
//              =======================
//              PA6     PA7     I2C1
//

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



int main(void)
{
    uint32_t i;
    uint8_t data;
    
    I2C1_MasterOpen();  // Initializer I2C1 for master operation
    I2C1_WriteData(Slave_ADDR, IODIRA_ADDR, 0x00);   // Configure IODIRA to all output
    
    for(;;)
    {
        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0xFF);   // Set GPIOA to all LOW
        for(i = 0; i < 400000; i++);
        I2C1_WriteData(Slave_ADDR, GPIOA_ADDR, 0x00);   // Set GPIOA to all HIGH
        for(i = 0; i < 400000; i++);
        
        // Read data from GPIOB
        I2C1_ReadData(Slave_ADDR, GPIOB_ADDR, &data);
        data = data;
        
    }
    
   
}



