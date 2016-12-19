/**
  * @file    bsp_ddas_vref.c
  * @brief   Board Support Package - DDAS Vref
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f4xx_hal.h"
#include "bsp_ddas.h"


static TIM_HandleTypeDef                htim;


static
void	bsp_ddas_vref_io_cfg( void )
{
	GPIO_InitTypeDef        gpio_vref       = {     .Pin            =   GPIO_PIN_9,
	                                                .Mode           =   GPIO_MODE_AF_PP,
	                                                .Pull           =   GPIO_NOPULL,
                                                        .Speed          =   GPIO_SPEED_FREQ_MEDIUM,
                                                        .Alternate      =   GPIO_AF1_TIM1 };

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init( GPIOE, &gpio_vref );
}

static
void bsp_ddas_vref_pwm_cfg(                     size_t                  pwm_carrier_hz )
{
        TIM_OC_InitTypeDef      tim_cmp;

        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  prescaler_tcks  =   0;
        size_t                  pwm_cycle_tcks  =   (apb2_clk_hz / (prescaler_tcks + 1)) / pwm_carrier_hz;
        size_t                  pwm_duty_tcks   =   pwm_cycle_tcks / 2;


        __HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_RCC_TIM1_FORCE_RESET();
        __HAL_RCC_TIM1_RELEASE_RESET();

        htim.Instance                   =   TIM1;
        htim.Init.Period                =   pwm_cycle_tcks;
        htim.Init.Prescaler             =   prescaler_tcks;
        htim.Init.ClockDivision         =   0;
        htim.Init.CounterMode           =   TIM_COUNTERMODE_UP;
        htim.Init.RepetitionCounter     =   0;

        if( HAL_TIM_PWM_Init( &htim ) != HAL_OK )
        {
                // TIM Initialization Error
        }

        tim_cmp.OCMode                  =   TIM_OCMODE_PWM2;
        tim_cmp.OCFastMode              =   TIM_OCFAST_DISABLE;
        tim_cmp.OCPolarity              =   TIM_OCPOLARITY_LOW;
        tim_cmp.OCNPolarity             =   TIM_OCNPOLARITY_HIGH;
        tim_cmp.OCIdleState             =   TIM_OCIDLESTATE_SET;
        tim_cmp.OCNIdleState            =   TIM_OCNIDLESTATE_RESET;
        tim_cmp.Pulse                   =   pwm_duty_tcks;

        if( HAL_TIM_PWM_ConfigChannel( &htim, &tim_cmp, TIM_CHANNEL_1 ) != HAL_OK )
        {
                //TIM CHNL Configuration Error
        }

        if( HAL_TIM_PWM_Start( &htim, TIM_CHANNEL_1 ) != HAL_OK )
        {
                //TIM Starting Error
        }
}

void bsp_ddas_vref_init(                        size_t                  pwm_carrier_hz )
{
        //bsp_ddas_vref_io_cfg();
        bsp_ddas_vref_pwm_cfg( pwm_carrier_hz );
}

void bsp_ddas_vref_enable(                      void )
{
	GPIO_InitTypeDef        gpio_vref       = {     .Pin            =   GPIO_PIN_9,
	                                                .Mode           =   GPIO_MODE_AF_PP,
	                                                .Pull           =   GPIO_NOPULL,
                                                        .Speed          =   GPIO_SPEED_FREQ_MEDIUM,
                                                        .Alternate      =   GPIO_AF1_TIM1 };

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init( GPIOE, &gpio_vref );
}

void bsp_ddas_vref_disable(                     void )
{
	GPIO_InitTypeDef        gpio_vref       = {     .Pin            =   GPIO_PIN_9,
	                                                .Mode           =   GPIO_MODE_INPUT,
	                                                .Pull           =   GPIO_NOPULL,
                                                        .Speed          =   GPIO_SPEED_FREQ_MEDIUM,
                                                        .Alternate      =   GPIO_AF1_TIM1 };

	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_Init( GPIOE, &gpio_vref );
}
