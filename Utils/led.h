//
// Created by fish on 2024/3/2.
//

#ifndef CANTEST_LED_H
#define CANTEST_LED_H

#include "can.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

void ledInit(TIM_HandleTypeDef *h);
void ledSet(int R, int G, int B);

#endif//CANTEST_LED_H
