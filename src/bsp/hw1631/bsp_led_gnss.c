/**
  * @file    bsp_led_gnss.h
  * @brief   Board Support Package - GNSS LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_led.h"
#include	"bsp.h"

/**
 * @brief GNSS LED pin initialization
 */
void	bsp_led_gnss_gps_init( void )
{
	GPIO_InitTypeDef        gpio_led_gnss_gps       =   {    .Pin       =    GPIO_PIN_10,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_gnss_gps    );
}

void	bsp_led_gnss_dgps_init( void )
{
	GPIO_InitTypeDef        gpio_led_gnss_dgps      =   {    .Pin       =    GPIO_PIN_9,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_gnss_dgps   );
}

void	bsp_led_gnss_rtkint_init( void )
{
	GPIO_InitTypeDef        gpio_led_gnss_rtkint    =   {    .Pin       =    GPIO_PIN_8,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_gnss_rtkint );
}

void	bsp_led_gnss_rtkflt_init( void )
{
	GPIO_InitTypeDef        gpio_led_gnss_rtkflt    =   {    .Pin       =    GPIO_PIN_7,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_led_gnss_rtkflt );
}

/**
 * @brief LED0 get state
 */
bool	bsp_led_gnss_gps_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_10 ) ? true : false );
}

/**
 * @brief LED0 get state
 */
bool	bsp_led_gnss_dgps_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_9 ) ? true : false );
}

/**
 * @brief LED0 get state
 */
bool	bsp_led_gnss_rtkint_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_8 ) ? true : false );
}

/**
 * @brief LED0 get state
 */
bool	bsp_led_gnss_rtkflt_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_7 ) ? true : false );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_gnss_gps_set(           const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_10,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_gnss_dgps_set(          const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_gnss_rtkint_set(        const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_gnss_rtkflt_set(        const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_7,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}
