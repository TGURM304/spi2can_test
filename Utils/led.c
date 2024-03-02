//
// Created by fish on 2024/3/2.
//

#include "led.h"

TIM_HandleTypeDef *ledTIM;

void ledInit(TIM_HandleTypeDef *h) {
  ledTIM = h;
  HAL_TIM_PWM_Start(h, TIM_CHANNEL_1); // B
  HAL_TIM_PWM_Start(h, TIM_CHANNEL_2); // G
  HAL_TIM_PWM_Start(h, TIM_CHANNEL_3); // R
}

void ledSet(int R, int G, int B) {
  __HAL_TIM_SetCompare(ledTIM, TIM_CHANNEL_1, B);
  __HAL_TIM_SetCompare(ledTIM, TIM_CHANNEL_2, G);
  __HAL_TIM_SetCompare(ledTIM, TIM_CHANNEL_3, R);
}