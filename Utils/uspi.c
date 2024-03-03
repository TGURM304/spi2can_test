//
// Created by fish on 2024/3/2.
//

#include "uspi.h"
#include "led.h"
#define min(a, b) ((a) < (b) ? (a) : (b))
#define BUFFER_LIMIT 10

SPI_HandleTypeDef *SPIh;
uint8_t spi_tx[BUFFER_LIMIT];

void spiInit(SPI_HandleTypeDef *h) {
  SPIh = h;
}

void spiSend(const uint8_t *data, uint8_t len, uint8_t control) {
  ledSet(255, 0, 0);
  if(control == 0) len = min(len, 8);
  spi_tx[0] = control, spi_tx[1] = len;
  for(int i = 0; i < len; i++) spi_tx[i+2] = data[i];
  HAL_SPI_Transmit(SPIh, spi_tx, BUFFER_LIMIT, 0xffff);
//  HAL_SPI_Transmit_DMA(SPIh, spi_tx, BUFFER_LIMIT);
}