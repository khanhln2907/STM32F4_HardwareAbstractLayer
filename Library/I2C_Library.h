#ifndef I2C_LIBRARY
#define I2C_LIBRARY
#include "GPIO_Library.h"

#define I2C_OK 1

#define I2C_PORT_ERROR 100

// Public function
int I2C_Config(I2C_TypeDef* i2cPort);
void I2C_transmit(I2C_TypeDef* i2cPort, uint8_t rxAdress, const uint8_t* txMsg, uint8_t txLen);
void I2C_receiveString(I2C_TypeDef* i2cPort, uint8_t txAdress, volatile uint8_t* rxMsg, uint8_t rxLen);
uint8_t I2C_receiveByte(I2C_TypeDef* i2cPort);



// Private functions for I2C Protocol

void _I2C_sendStart(I2C_TypeDef* i2cPort);
void _I2C_sendAddress(I2C_TypeDef* i2cPort, uint8_t Address);
void _I2C_sendMultipleByte(I2C_TypeDef* i2cPort, uint8_t* txMsg, uint8_t txLen);
void _I2C_sendSingleByte(I2C_TypeDef* i2cPort, uint8_t dataByte);
void _I2C_sendStop(I2C_TypeDef* i2cPort);

// After transmitting ADDR bit, call this function to read Data from Slave
uint8_t _I2C_readByte(I2C_TypeDef* i2cPort);

#endif

