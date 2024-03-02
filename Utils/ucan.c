//
// Created by fish on 2024/3/2.
//

#include "ucan.h"
#include "string.h"

void CanInit(CAN_HandleTypeDef *h) {
  // 开启 CAN_IT_RX_FIFO0_MSG_PENDING 接收中断
  CAN_FilterTypeDef filter;
  filter.FilterActivation = CAN_FILTER_ENABLE;
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterFIFOAssignment = CAN_FilterFIFO0;
  filter.FilterIdHigh = 0x0000;
  filter.FilterIdLow = 0x0000;
  filter.FilterMaskIdHigh = 0x0000;
  filter.FilterMaskIdLow = 0x0000;
  filter.SlaveStartFilterBank = 14;
  HAL_CAN_ConfigFilter(h, &filter);
  HAL_CAN_Start(h);
  HAL_CAN_ActivateNotification(h, CAN_IT_RX_FIFO0_FULL);
}

char receiveData[50];
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *h) {
  // 把 CAN 接收到的信息转发到串口
  CAN_RxHeaderTypeDef rh;
  HAL_CAN_GetRxMessage(h, CAN_RX_FIFO0, &rh, (uint8_t *)receiveData);
  HAL_UART_Transmit(&huart6, (uint8_t *)receiveData, strlen(receiveData), 0xffff);
}