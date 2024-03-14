//
// Created by fish on 2024/3/2.
//

#include "uspi.h"
#include "led.h"
#include "common.h"


SPI_HandleTypeDef *SPIh;
uint8_t spi_tx[BUFFER_SIZE];

void spiInit(SPI_HandleTypeDef *h) {
  SPIh = h;
}

void spiSend(const uint8_t *data, uint8_t len, uint8_t control) {

  // software cs
//  HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);

  if(control == 0) len = min(len, 8);
  spi_tx[0] = 0xff;
  spi_tx[1] = control, spi_tx[2] = len;
  for(int i = 0; i < len; i++) spi_tx[i+3] = data[i];
  for(int i = len+3; i < BUFFER_SIZE; i++) spi_tx[i] = 0;

  HAL_SPI_Transmit_DMA(SPIh, spi_tx, BUFFER_SIZE);
//  HAL_SPI_Transmit(SPIh, spi_tx, BUFFER_LIMIT, 0xffff);

  // software cs
//  HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
}

uint8_t GetStartPos(uint8_t StdId) {
  switch(StdId) {
    case 0x01:
      return 0;
    case 0x02:
      return 8;
    case 0x03:
      return 16;
    case 0x04:
      return 24;
    case 0x05:
      return 32;
    case 0x06:
      return 40;
    default:
      return 50;
  }
}

uint8_t spi_rx[BUFFER_SIZE];
uint8_t ids[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

// Counter
uint64_t suc_cnt = 0, err_cnt = 0, spi_cnt = 0;
extern uint64_t can_tx_cnt, can_rx_cnt, can_rx_suc_cnt, can_rx_err_cnt;

void spiTransmitReceive(const uint8_t *data, uint8_t len, uint8_t control) {
  uint8_t _xor = 0; // 按位异或得到校验码，存到最后一位

  if(control == 0) len = min(len, 8);
  spi_tx[0] = control, spi_tx[1] = len;
  for(int i = 0; i < len; i++) spi_tx[i+2] = data[i], _xor ^= data[i];
  for(int i = len+2; i < BUFFER_SIZE - 1; i++) spi_tx[i] = 0;
  spi_tx[BUFFER_SIZE - 1] = _xor;

  HAL_SPI_TransmitReceive_DMA(SPIh, spi_tx, spi_rx, BUFFER_SIZE);
  HAL_Delay(1);
//  for(int i = 1; i < 7; i++) spi_rx[GetStartPos(i)] = 0xff;

//  HAL_UART_Transmit(&huart6, spi_rx, BUFFER_LIMIT, 0xffff);

  // count
  uint8_t t_err_cnt = 0;
  for(int i = 0; i < 6; i++) {
    uint8_t st = GetStartPos(ids[i]);
    uint8_t fl = 1;
    for(int j = 1; j < 8; j++) {
      if(spi_rx[st + j] != spi_rx[st + j - 1] + 1) {
        fl = 0;
        t_err_cnt++;
      }
    }
    // 校验通过则为绿灯，否则为红灯
    if(fl) ledSet(0, 255, 0);
    else ledSet(255, 0, 0);
    suc_cnt += fl, err_cnt += !fl;
  }

  spi_cnt++;

  char t[100]; sprintf(t, "can_tx_cnt = %lld, spi_cnt = %lld, suc_cnt = %lld, err_cnt = %lld, t_err_cnt = %d\r\n", can_tx_cnt, spi_cnt, suc_cnt, err_cnt, t_err_cnt);
  HAL_UART_Transmit(&huart6, (uint8_t *)t, strlen(t), 0xffff);
}