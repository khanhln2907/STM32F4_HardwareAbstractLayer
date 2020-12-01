#ifndef SPI_LIBRARY
#define SPI_LIBRARY

#include "stm32f429xx.h"
#include "GPIO_Library.h"

#define SPI_OK 1

#define SPI_ERROR 100
#define SPI_PORT_UNDEFINED SPI_ERROR + 1
#define SPI_DATA_FORMAT_UNDEFINED SPI_PORT_UNDEFINED + 1


typedef enum {
	SPI_MASTER = 0b1, 	
	SPI_SLAVE = 0b0 	
}SPI_opMode;	


typedef enum {
	SPI_2_LINE = 0b0, 	// full-duplex -> RXONLY = 0
	
	// Half-duplex, in this mode, modify the bit BIDIOE to select transmit
	// and receive direction
	SPI_1_LINE = 0b1, 	
}SPI_wireMode;	

typedef enum {
	SPI_MSBFIRST = 0b0, 	
	SPI_LSBFIRST = 0b1, 	
}SPI_dataFormat;	

typedef enum {
	SPI_CPOL_0 = 0b0, 	// 0 when idle
	SPI_CPOL_1 = 0b1	// 1 when idle
}SPI_clockPolarity;

typedef enum {
	SPI_CPHA_0_RISING_EDGE = 0b0, 	// First clock transition
	SPI_CPHA_1_FALLING_EGDE = 0b1	// Second clock transition
}SPI_clockPhase;	

// APB1: SPI2, SPi3
// APB2: SPI1, 4, 5, 6

typedef enum {
	SPI_BAUD_DIV_2 = 0b000,
	SPI_BAUD_DIV_4 = 0b001,
	SPI_BAUD_DIV_8 = 0b010,
	SPI_BAUD_DIV_16 = 0b011,
	SPI_BAUD_DIV_32 = 0b100,
	SPI_BAUD_DIV_64 = 0b101,
	SPI_BAUD_DIV_128 = 0b110,
	SPI_BAUD_DIV_256 = 0b111	
}SPI_baudPrescaler;	


typedef struct {
	SPI_opMode operationMode;
	SPI_wireMode wireMode;
	SPI_dataFormat dataFormat;
	SPI_clockPolarity clockPol;
	SPI_clockPhase clockPhase;
	SPI_baudPrescaler baudPrescaler;
	bool softwareNSSEnable; 
} SPI_GeneralConfigType;


void SPI_Config(SPI_TypeDef* spiPort, SPI_GeneralConfigType spiConfig);

void SPI_transmitByte(SPI_TypeDef* spiPort, uint8_t txByte);

void SPI_transmitString(SPI_TypeDef* spiPort, uint8_t* txMsg, uint8_t txLen);

void SPI_DisableTransmission(SPI_TypeDef* spiPort);


#endif