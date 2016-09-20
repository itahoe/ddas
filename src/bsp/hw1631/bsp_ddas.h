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


void bsp_ddas_adc_init( void );

void bsp_ddas_adc_smplrate_set(                 size_t                  smplrate_sps );

void bsp_ddas_adc_start(                        uint16_t *              data_0,
                                                uint16_t *              data_1,
                                                size_t                  size );

void bsp_ddas_adc_dma_isr( void );

void bsp_ddas_vref_init(                        size_t                  pwm_carrier_hz );


#endif	//BSP_DDAS_H
