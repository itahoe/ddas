/**
  * @file    bsp_flog.c
  * @brief   Board Support Package - File Manager Low Level Service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_flog.h"
#include	"bsp.h"

/**
 * @brief BSP File Manager initialization
 */
void	bsp_flog_init( void )
{
/*
	EXTI->IMR       |=  BSP_IRQ_EXTI_2;
	HAL_NVIC_SetPriority( EXTI2_IRQn, BSP_NVIC_PRIO_FLOG, 0 );
	HAL_NVIC_EnableIRQ( EXTI2_IRQn );
*/
}
