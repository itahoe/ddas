/**
  * @file    bsp_pmu.h
  * @brief   BSP - PMU - Sensor VBAT
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_pmu.h"
#include	"bsp.h"


static
void bsp_pmu_sts_init( void )
{
	GPIO_InitTypeDef        gpio_sts_charge =   {   .Pin    =   GPIO_PIN_15,
	                                                .Mode   =   GPIO_MODE_INPUT,
	                                                .Pull   =   GPIO_PULLUP,
	                                                .Speed  =   GPIO_SPEED_FREQ_LOW };

	GPIO_InitTypeDef        gpio_sts_pgood  =   {   .Pin    =   GPIO_PIN_6,
	                                                .Mode   =   GPIO_MODE_INPUT,
	                                                .Pull   =   GPIO_PULLUP,
	                                                .Speed  =   GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_Init( GPIOB, &gpio_sts_charge );
	HAL_GPIO_Init( GPIOC, &gpio_sts_pgood );
}

/**
 * @brief BSP PMU Sens VBAT Initialization.
 */
void bsp_pmu_sens_vbat_init( void )
{



}

/**
 * @brief BSP PMU Sens VBAT Control
 */
void bsp_pmu_sens_vbat_enable( bool  bval )
{
	//HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

/**
 * @brief BSP PMU Sens VBAT Get
 */
bool bsp_pmu_sens_vbat_get( void )
{
	//return( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_15 ) != GPIO_PIN_SET ? true : false );
}
