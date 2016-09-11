/**
  * @file    bsp.h
  * @brief   Board Support Package
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_H
#define  BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "app_trace.h"
#include "stm32f4_lll.h"
//#include "stm32f4xx_hal.h"

#define BSP_SYSTICK_HZ                  1000ul

#define BSP_NVIC_PRIO_GNSS_DMA_RX       9
#define BSP_NVIC_PRIO_GNSS_DMA_TX       9
#define BSP_NVIC_PRIO_GNSS_RECV_SMBL    11
#define BSP_NVIC_PRIO_SDIO              5
#define BSP_NVIC_PRIO_SDIO_DMA_RX       7
#define BSP_NVIC_PRIO_SDIO_DMA_TX       7
#define BSP_NVIC_PRIO_USB_CDC_TMR_POLL  13

#define BSP_IRQ_EXTI_0                  (1 << 0)
#define BSP_IRQ_EXTI_1                  (1 << 1)
#define BSP_IRQ_EXTI_2                  (1 << 2)
#define BSP_IRQ_EXTI_3                  (1 << 3)

#endif	//BSP_H
