/**
  * @file    bsp_ddas.h
  * @brief   Board Support Package - DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BSP_DDAS_H
#define  BSP_DDAS_H


#include <stdbool.h>
#include <stdio.h>
#include "bsp.h"


void bsp_ddas_init(                     const   int16_t *               data,
                                                size_t                  size );

void bsp_ddas_start( void );

void bsp_ddas_dma_isr( void );

void bsp_ddas_vref_init(                        size_t                  pwm_carrier_hz );

#endif	//BSP_DDAS_H
