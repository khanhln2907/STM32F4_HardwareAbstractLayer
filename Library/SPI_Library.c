#include "SPI_Library.h"
#include <assert.h>

/****************************************************************
//SPI1:	
//- NSS:  PA4, PA15
//- SCK:  PA5, PB3
//- MISO: PA6, PB4
//- MOSI: PA7, PB5

//SPI2:	
//- NSS:  PB9,  PB12, PI0
//- SCK:  PB10, PB13, PD3, PI1
//- MISO: PB14, PC2,  PI2
//- MOSI: PB15, PC3,  PI3

//SPI3:	
//- NSS:  PA4, PA15
//- SCK:  PB3, PC10
//- MISO: PB4, PC11
//- MOSI: PB5, PC12, PD6

//SPI4:	
//- NSS:  PE4, PE11
//- SCK:  PE2, PE12
//- MISO: PE5, PE13
//- MOSI: PE6, PE14

//SPI5:	
//- NSS:  PF6, PH5, PC1
//- SCK:  PF7, PH6
//- MISO: PF8, PH7
//- MOSI: PF9, PF11, 

//SPI6:	
//- NSS:  PG8 
//- SCK:  PG13
//- MISO: PG12
//- MOSI: PG14	
*****************************************************************/

/*
TODO:
- Function SPI_DisableTransmission blocks until the bus is no more busy (blocking core)
- Read SPI is not yet implemented
- Chip selection was not carefully considered
- Port initialization might be integrated (right now SPI and PORT are configured independently
*/

void SPI_Config(SPI_TypeDef* spiPort, SPI_GeneralConfigType spiConfig){
	// Enable Clock
	if(spiPort == SPI1){
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	} else if (spiPort == SPI2){
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	} else if (spiPort == SPI3){
		RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	} else if (spiPort == SPI4){
		RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	} else if (spiPort == SPI5){
		RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;	
	} else if (spiPort == SPI6){
		RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;	
	} else {
		assert(!SPI_PORT_UNDEFINED);
	}
	
	uint16_t BIDIMODE = spiConfig.wireMode << SPI_CR1_BIDIMODE_Pos;
	uint16_t MSTR = spiConfig.operationMode << SPI_CR1_MSTR_Pos;
	uint16_t SSM = spiConfig.softwareNSSEnable << SPI_CR1_SSM_Pos;
	uint16_t CPOL = spiConfig.clockPol << SPI_CR1_CPOL_Pos;
	uint16_t CPHA = spiConfig.clockPhase << SPI_CR1_CPHA_Pos;
	uint16_t BR = spiConfig.baudPrescaler << SPI_CR1_BR_Pos;
	uint16_t LSBFIRST = spiConfig.dataFormat << SPI_CR1_LSBFIRST_Pos;
	
	// Setup resister according to the custom configuration
	uint16_t SPICR1R = (uint16_t) BIDIMODE | MSTR | SSM | CPOL | CPHA | BR | LSBFIRST;
	spiPort->CR1 = SPICR1R;
	
	// SSOE ? 
	spiPort->CR2 |= SPI_CR2_SSOE; // Not sure
	
	// Enable peripheral
	spiPort->CR1 |= SPI_CR1_SPE;
}


void SPI_transmitByte(SPI_TypeDef* spiPort, uint8_t txByte){
	// Enable transmission
	spiPort->CR1 |= SPI_CR1_SPE;
	
	spiPort->DR = txByte;
	while(!(spiPort->SR & SPI_SR_TXE));
}

	
void SPI_transmitString(SPI_TypeDef* spiPort, uint8_t* txMsg, uint8_t txLen){
	// Enable transmission
	spiPort->CR1 |= SPI_CR1_SPE;
	
	for (int i = 0; i < txLen; i++){
		spiPort->DR = txMsg[i];
		while(!(spiPort->SR & SPI_SR_TXE));	
	}
	
	// This part blocks until transmission is completed so that user can turn off Slave selection port
	SPI_DisableTransmission(spiPort);
}




void SPI_DisableTransmission(SPI_TypeDef* spiPort)
{
	// Wait until transmission is completed
	while(spiPort->SR & SPI_SR_BSY);
	//while(!(spiPort->SR & SPI_SR_TXE));
	
	//spiPort->CR1 &= ~SPI_CR1_SPE;
}


