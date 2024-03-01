//
// Created by fish on 2024/3/2.
//

#ifndef CANTEST_UCAN_H
#define CANTEST_UCAN_H

#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "can.h"
#include "usart.h"

void CanInit(CAN_HandleTypeDef *h);

#endif//CANTEST_UCAN_H
