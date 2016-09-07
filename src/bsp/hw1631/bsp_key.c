/**
  * @file    bsp_key.h
  * @brief   Board Support Package - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"bsp_key.h"
#include	"bsp.h"


/**
 * @brief BSP key initialization.
 */
void	bsp_key_func_init( void )
{
	GPIO_InitTypeDef        gpio_key_func   =   {   .Pin    =   GPIO_PIN_15,
	                                                .Mode   =   GPIO_MODE_INPUT,
	                                                .Pull   =   GPIO_PULLDOWN,
	                                                .Speed  =   GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init( GPIOE, &gpio_key_func );
}

/**
 * @brief BSP key state get
 */
bool	bsp_key_func_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOE, GPIO_PIN_15 ) ? true : false );
}
