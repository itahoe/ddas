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


void bsp_ddas_init(                             void );

int bsp_ddas_start(                     const   int16_t *       p,
                                                size_t          size );

void bsp_ddas_dma_isr(                          void );

#endif	//BSP_DDAS_H
