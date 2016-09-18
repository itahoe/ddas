/**
  * @file    ddas.c
  * @brief   DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bsp_ddas.h"
#include "ddas.h"


/**
 * @brief DDAS initialization
 */
void ddas_init(                             ddas_t *            p )
{
        size_t          pwm_carrier_hz  =   1000000;

	bsp_ddas_vref_init( pwm_carrier_hz );
	bsp_ddas_adc_init();
}

void ddas_start(                            ddas_t *            p )
{
	bsp_ddas_adc_smplrate_set( p->adc_smplrate_sps );
	bsp_ddas_adc_start( p->data, p->size );
}

void ddas_adc_dma_isr( void )
{
        bsp_ddas_adc_dma_isr();
}

