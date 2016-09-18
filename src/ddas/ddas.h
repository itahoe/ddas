/**
  * @file    ddas.h
  * @brief   DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  DDAS_H
#define  DDAS_H


#include "config.h"
#include "bsp_ddas.h"


typedef	uint32_t                        ddas_smpl_t;


typedef	struct	ddas_s
{
        size_t                  adc_smplrate_sps;
        ddas_smpl_t *           data;
	size_t                  size;
} ddas_t;


void ddas_init(                                 ddas_t *        p );

void ddas_start(                                ddas_t *        p );

void ddas_recv_hook(                            ddas_t *        p );

void ddas_adc_dma_isr( void );

#endif	//DDAS_H
