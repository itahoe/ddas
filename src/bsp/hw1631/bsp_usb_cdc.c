/**
  * @file    bsp_usb_cdc.c
  * @brief   BSP - USB CDC
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_usb.h"
#include	"bsp.h"

/**
 * @brief BSP USB CDC Initialization
 */
void bsp_usb_cdc_init( void )
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM3_FORCE_RESET();
	__HAL_RCC_TIM3_RELEASE_RESET();

	HAL_NVIC_SetPriority( TIM3_IRQn, BSP_NVIC_PRIO_USB_CDC_TMR_POLL, 0 );
	HAL_NVIC_EnableIRQ( TIM3_IRQn );
}
