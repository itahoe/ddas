/**
  * @file    bsp_ddas_vref.c
  * @brief   Board Support Package - DDAS Vref
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_ddas.h"


void bsp_ddas_vref_init(                        size_t                  pwm_carrier_hz )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        uint16_t                pwm_cycle_tcks  =   apb2_clk_hz / pwm_carrier_hz;
        uint16_t                pwm_duty        =   pwm_cycle_tcks / 2;


        //GPIO
	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOEEN );
	__DSB();

	stm32f4_gpio_cfg_pin(           GPIOE,
                                        STM32F4_GPIO_PIN_09,
                                        STM32F4_GPIO_CFG_ALT_PP_SPEED_MEDIUM );

	stm32f4_gpio_cfg_af(            GPIOE,
                                        STM32F4_GPIO_PIN_09,
                                        STM32F4_GPIO_AF1_TIM1 );


        //TIM
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	__DSB();

	RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
	__DSB();

	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;
	__DSB();

	stm32f4_tmr_cfg(                TIM1,
                                        STM32F4_TMR_CKDIV_1 );

	stm32f4_tmr_cfg_chnl_update(    TIM1,
                                        STM32F4_TMR_CHNL_UPD_EVT_COMG_OR_TRGI );

	stm32f4_tmr_cfg_chnl_preload(   TIM1,
                                        STM32F4_TMR_CHNL_PRELOAD_ENABLE );

	stm32f4_tmr_cnt_cfg(            TIM1,
	                                STM32F4_TMR_CNT_PRELOAD_DISABLE,
	                                STM32F4_TMR_CNT_MODE_UPDN_MODE1_NONSTOP,
	                                //STM32F4_TMR_CNT_MODE_UP_NONSTOP,
	                                STM32F4_TMR_CNT_UPD_EVT_SRC_ANY );


	stm32f4_tmr_chnl_cfg_mode(      TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_CHNL_MODE_COMPARE );
	stm32f4_tmr_cmp_cfg_fast(       TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_CMP_FAST_DISABLE );
	stm32f4_tmr_cmp_cfg_preload(    TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_CMP_PRELOAD_ENABLE );
	stm32f4_tmr_cmp_cfg_mode(       TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_CMP_MODE_PWM_POSITIVE );
	stm32f4_tmr_cmp_cfg_clr(        TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_CMP_CLR_DISABLE );

	stm32f4_tmr_prescaler_set(      TIM1, 0x0000 );
	stm32f4_tmr_period_set(         TIM1, pwm_cycle_tcks - 1);

	stm32f4_tmr_cmp_set(            TIM1,
                                        STM32F4_TMR_CHNL_1,
                                        pwm_duty - 1 );

        stm32f4_tmr_out_p_cfg(          TIM1, STM32F4_TMR_CHNL_1, STM32F4_TMR_OUT_DIRECT );

	stm32f4_tmr_enable(             TIM1 );


	TIM1->EGR       =   TIM_EGR_COMG | TIM_EGR_UG;
        //master output enable
	TIM1->BDTR      =   TIM_BDTR_AOE | TIM_BDTR_MOE;
}
