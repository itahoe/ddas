/**
  * @file    bsp_ddas_vref.c
  * @brief   Board Support Package - DDAS Vref
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f4_lll.h"
#include "bsp_ddas.h"


static  size_t                          prescaler_tcks  =   0;


static
void	bsp_ddas_vref_io_cfg( void )
{
	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOEEN );
	__DSB();

	stm32f4_gpio_pin_cfg(   GPIOE,
                                STM32F4_GPIO_PIN_09,
                                STM32F4_GPIO_CFG_ALT_PP_SPEED_MEDIUM,
                                STM32F4_GPIO_AF1_TIM1 );
}

static
void bsp_ddas_vref_pwm_cfg(                     size_t                  pwm_carrier_hz )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  pwm_cycle_tcks  =   (apb2_clk_hz / (prescaler_tcks + 1)) / pwm_carrier_hz;
        size_t                  pwm_duty_tcks   =   pwm_cycle_tcks / 2;


        //TIM
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	__DSB();

	RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
	__DSB();

	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;
	__DSB();

	stm32f4_tmr_cfg(                TIM1,
                                        STM32F4_TMR_CKDIV_1,
                                        STM32F4_TMR_CHNL_DMA_EVT_DEFAULT,
                                        STM32F4_TMR_CHNL_UPD_EVT_COMG_OR_TRGI,
                                        STM32F4_TMR_CHNL_PRELOAD_ENABLE );

	stm32f4_tmr_cnt_cfg(            TIM1,
	                                STM32F4_TMR_CNT_PRELOAD_DISABLE,
	                                STM32F4_TMR_CNT_MODE_UP_NONSTOP,
	                                STM32F4_TMR_CNT_UPD_EVT_SRC_ANY );

	stm32f4_tmr_chnl_cfg_mode(      TIM1,
                                        STM32F4_TMR_CHNL_1,
                                        STM32F4_TMR_CHNL_MODE_COMPARE );

        stm32f4_tmr_cmp_cfg(            TIM1,
                                        STM32F4_TMR_CHNL_1,
                                        STM32F4_TMR_CMP_FAST_DISABLE,
                                        STM32F4_TMR_CMP_PRELOAD_ENABLE,
                                        STM32F4_TMR_CMP_MODE_PWM_POSITIVE,
                                        STM32F4_TMR_CMP_CLR_DISABLE );

        stm32f4_tmr_out_p_cfg(          TIM1,
                                        STM32F4_TMR_CHNL_1,
                                        STM32F4_TMR_OUT_DIRECT );

	stm32f4_tmr_prescaler_set(      TIM1, prescaler_tcks );

	stm32f4_tmr_period_set(         TIM1, pwm_cycle_tcks - 1);

	stm32f4_tmr_cmp_set(            TIM1, STM32F4_TMR_CHNL_1, pwm_duty_tcks - 1 );


	stm32f4_tmr_enable(             TIM1 );


	TIM1->EGR       =   TIM_EGR_COMG | TIM_EGR_UG;
        //master output enable
	TIM1->BDTR      =   TIM_BDTR_AOE | TIM_BDTR_MOE;
}

void bsp_ddas_vref_init(                        size_t                  pwm_carrier_hz )
{

        bsp_ddas_vref_io_cfg();
        bsp_ddas_vref_pwm_cfg( pwm_carrier_hz );
}
