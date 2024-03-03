//
// Created by fish on 2024/3/2.
//

#ifndef CANTEST_USPI_H
#define CANTEST_USPI_H

#include "can.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void spiInit(SPI_HandleTypeDef *h);
void spiSend(const uint8_t *data, uint8_t len, uint8_t control);

#endif//CANTEST_USPI_H
