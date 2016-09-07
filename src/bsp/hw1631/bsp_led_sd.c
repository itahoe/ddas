/**
  * @file    bsp_led.h
  * @brief   Board Support Package - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_led.h"
#include	"bsp.h"

/**
 * @brief LED0 pin initialization
 */
void	bsp_led_sd_init( void )
{
	GPIO_InitTypeDef        gpio_led_sd     =   {    .Pin       =    GPIO_PIN_9,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_Init( GPIOB, &gpio_led_sd );
}

/**
 * @brief LED0 state get.
 */
bool	bsp_led_sd_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_9 ) ? true : false );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_sd_set(                 const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}
