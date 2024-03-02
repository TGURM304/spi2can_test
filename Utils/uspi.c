//
// Created by fish on 2024/3/2.
//

#include "uspi.h"

SPI_HandleTypeDef *SPIh;
uint8_t spi_tx[10];

void spiInit(SPI_HandleTypeDef *h) {
  SPIh = h;
  //HAL_SPI_Transmit_DMA(h, spi_tx, 9);
}

void spiSend(const uint8_t *data, int len, int control) {
  if(len + 1 > 9) len = 9 - 1;
  spi_tx[0] = control;
  for(int i = 0; i < 9; i++) {
    if(i < len) spi_tx[i+1] = data[i];
    else spi_tx[i+1] = 0;
  }
  HAL_SPI_Transmit(SPIh, spi_tx, 9, 0xffff);
}