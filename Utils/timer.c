//
// Created by fish on 2024/3/2.
//

#include "timer.h"

void TimInit(TIM_HandleTypeDef *h) {
  HAL_TIM_Base_Start_IT(h);
}

char sendData[] = "qwq";

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *h) {
  // 定时中断 spi 发送
  if(h == &htim2)
    HAL_SPI_Transmit(&hspi2, (uint8_t *)sendData, strlen(sendData), 0xffff);
}