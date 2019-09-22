// File: BMP_280_Driver.c
// Title:       BMP280 Driver for TM4C123GH6PM
// Description: Implements I2C using CMSIS.only I2C module 1 is implemented.
//              The clock speed (SCL) is 100 KHz
//
//              Functions:
//              I2Cx_MasterOpen
//              I2Cx_WriteData
//              I2Cx_ReadData

  
#include "tm4c123gh6pm.h"
#include "BMP_280_Driver.h"

// void BMP280MasterOpen(void)
// Opens I2C Module 1 with clock speed of 100 kHz assuming bus speed is 16 MHz
// PA6 - I2C1 SCL 
// PA7 - I2C1 SDA
void BMP280MasterOpen(void)
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

// void BMP280BeginTransmission(uint8_t slaveAddress, uint8_t rs)
// Set's the slave's address into Master Slave Address register
// uint8_t slaveAddress : address of slave
// uint8_t rs : receive or send bit.
//              0 = Transmit
//              1 = Receive
static void BMP280BeginTransmission(uint8_t slaveAddress, uint8_t rs)
{

    I2C1->MSA = (slaveAddress << 1) + rs;   
}

// static uint8_t BMP280WriteByte(uint8_t data, uint8_t start, uint8_t run, uint8_t stop)
// Sends a byte to the slave device.
// uint_8t data  : data to send to slave device. Can be actualy payload data or register address
// uint8_t start : 
// uint8_t run   :
// uint8_t stop  :
// return: uint8_t return value. 0 = no error, 1 = error error has occured
static uint8_t BMP280WriteByte(uint8_t data, uint8_t start, uint8_t run, uint8_t stop)
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

// uint8_t BMP280WriteData(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
// uint8_t slaveAddress : address of the slave device
// uint8_t regAddress   : address of the register to write the data to
// uint8_t data         : data to write to the register
// return: uint8_t return value. 0 = no error. 1 = error while setting register address, 2 = error while writing data to register address
uint8_t BMP280WriteData(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
{
    uint8_t error = 0;
    BMP280BeginTransmission(slaveAddress, 0);        //  Set slave address to write to
    error = BMP280WriteByte(regAddress, 1, 1, 0);    //  Set register address to write to
    if(error != 0)
    {
        error = 1;
        return error;
    }
    
    error = BMP280WriteByte(data, 0, 1, 1);          //  Send data to specified register address of slave device 
        if(error != 0)
    {
        error = 2;
        return error;
    }
    
    return error;
}

// uint8_t BMP280ReadData(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data)
// uint8_t slaveAddress : address of the slave device
// uint8_t regAddress   : address of the register to write the data to
// uint8_t *data        : pointer to where to store data to
// return: uint8_t return value. 0 = no error. 1 = error while setting register address, 2 = error while writing data to register address
uint8_t BMP280ReadData(uint8_t slaveAddress, uint8_t regAddress, uint8_t *data)
{
    uint8_t error = 0;
    BMP280BeginTransmission(slaveAddress, 1);        //  Set slave address to read from
    error = BMP280WriteByte(regAddress, 1, 1, 1);    //  Set register address to read from
    if(error != 0)
    {
        error = 1;
        return error;
    }
    *data = I2C1->MDR;
    return error;
}

