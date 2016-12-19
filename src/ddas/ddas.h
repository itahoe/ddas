/**
  * @file    ddas.h
  * @brief   DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  DDAS_H
#define  DDAS_H


#include "config.h"
#include "bsp_ddas.h"


typedef	uint16_t                        ddas_smpl_t;
typedef	uint8_t                         ddas_pckt_sync_t;
typedef	uint8_t                         ddas_pckt_size_t;
typedef	uint16_t                        ddas_pckt_spare_t;

#define DDAS_PCKT_DATA_SIZE_OCT         CFG_DDAS_PCKT_SIZE_OCT
#define DDAS_PCKT_TILE_SIZE_OCT         ( sizeof(ddas_pckt_sync_t) + \
                                          sizeof(ddas_pckt_size_t) + \
                                          sizeof(ddas_pckt_spare_t) )

#define DDAS_PCKT_DATA_SIZE_SMPL        (DDAS_PCKT_DATA_SIZE_OCT / sizeof(ddas_smpl_t))

typedef	struct	ddas_pckt_s
{
        ddas_smpl_t             data[ DDAS_PCKT_DATA_SIZE_SMPL ];
        ddas_pckt_sync_t        sync;
        ddas_pckt_size_t        size;
        ddas_pckt_spare_t       spare;
} ddas_pckt_t;

typedef	struct	ddas_s
{
        size_t                  adc_smplrate_sps;
        //ddas_smpl_t *           data;
        ddas_smpl_t *           data_0;
        ddas_smpl_t *           data_1;
	//size_t                  size;
        size_t                  blck_num;
} ddas_t;


void ddas_init(                                 ddas_t *        p );

void ddas_start(                                ddas_t *        p );

void ddas_recv_hook(                            ddas_t *        p );

void ddas_vref_enable(                          void );

void ddas_vref_disable(                         void );

void ddas_adc_dma_isr( void );

#endif	//DDAS_H
