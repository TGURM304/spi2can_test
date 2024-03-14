//
// Created by fish on 2024/3/2.
//

#include "ucan.h"
#include "string.h"
#include "led.h"

uint64_t can_tx_cnt, can_rx_cnt, can_rx_suc_cnt, can_rx_err_cnt;

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
  HAL_CAN_ActivateNotification(h, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void CanSend(CAN_HandleTypeDef *h, uint8_t *data, uint8_t StdId) {
  CAN_TxHeaderTypeDef can_tx_message;
  uint32_t send_mail_box;
  can_tx_message.StdId = StdId;
  can_tx_message.IDE = CAN_ID_STD;
  can_tx_message.RTR = CAN_RTR_DATA;
  can_tx_message.DLC = 0x08;
  HAL_CAN_AddTxMessage(h, &can_tx_message, data, &send_mail_box);
  can_tx_cnt++;
}

char receiveData[50];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *h) {
  // 把 CAN 接收到的信息转发到串口
//  ledSet(0, 255, 0);
  CAN_RxHeaderTypeDef rh;
  HAL_CAN_GetRxMessage(h, CAN_RX_FIFO0, &rh, (uint8_t *)receiveData);
//  HAL_UART_Transmit(&huart6, (uint8_t *)receiveData, strlen(receiveData), 0xffff);
  uint8_t fl = 1;
  for(int i = 0; i < 8; i++) if(receiveData[i] != i) fl = 0;
  can_rx_suc_cnt += fl, can_rx_err_cnt += !fl;
  can_rx_cnt++;
}