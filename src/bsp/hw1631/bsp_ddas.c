/**
  * @file    bsp_ddas.c
  * @brief   Board Support Package - DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

//#include	<string.h>
#include	"bsp_ddas.h"
//#include	"bsp.h"
//#include	"config.h"


static	ADC_HandleTypeDef		hadc;
static	DMA_HandleTypeDef               hdma;
static  TIM_HandleTypeDef               htim;


static
void bsp_ddas_init_timebase(                    uint32_t                smplrate_sps )
{
/*
        int                             resp            =   0;
        TIM_MasterConfigTypeDef         htim_cfg;
        uint32_t                        apb2_clk_hz     =   168000000 / 2;


        __HAL_RCC_TIM8_CLK_ENABLE();

        htim.Instance                   =   TIM8;
        htim.Init.Period                =   (apb2_clk_hz / smplrate_sps) - 1;
        htim.Init.Prescaler             =   1000;
        htim.Init.ClockDivision         =   0;
        htim.Init.CounterMode           =   TIM_COUNTERMODE_UP;
        htim.Init.RepetitionCounter     =   0;

        if( HAL_TIM_Base_Init( &htim ) != HAL_OK )
        {
                resp            =   -1;
        }

        //TIM8 TRGO selection
        htim_cfg.MasterOutputTrigger    =   TIM_TRGO_UPDATE;
        htim_cfg.MasterSlaveMode        =   TIM_MASTERSLAVEMODE_DISABLE;

        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &htim_cfg ) != HAL_OK )
        {
                resp            =   -1;
        }

        return( resp );
*/

/*
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	__DSB();

	RCC->APB2RSTR |= RCC_APB2RSTR_TIM8RST;
	__DSB();

	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM8RST;
	__DSB();

	stm32f4_tmr_cfg_ckdiv(          TIM8, STM32F4_TMR_CKDIV_1 );

	stm32f4_tmr_cnt_cfg(            TIM8,
	                                STM32F4_TMR_CNT_PRELOAD_DISABLE,
	                                STM32F4_TMR_CNT_MODE_UPDN_MODE1_NONSTOP,
	                                STM32F4_TMR_CNT_UPD_EVT_SRC_ANY );

	stm32f4_tmr_cfg_chnl_dma_evt(   TIM8, STM32F4_TMR_CHNL_DMA_EVT_CCx );
	stm32f4_tmr_cfg_chnl_update(    TIM8, STM32F4_TMR_CHNL_UPD_EVT_COMG_OR_TRGI );
	stm32f4_tmr_cfg_chnl_preload(   TIM8, STM32F4_TMR_CHNL_PRELOAD_ENABLE );

	stm32f4_tmr_cfg_slave(          TIM8,
                                        STM32F4_TMR_SMCR_ETRP_POSEDGE_DIV1,
                                        STM32F4_TMR_SMCR_ETRF_DIV1_N1,
                                        STM32F4_TMR_SMCR_MSM_DISABLE,
                                        STM32F4_TMR_SMCR_TRG_SEL_DEFAULT,
                                        STM32F4_TMR_SMCR_SLV_MODE_RESET );
                                        //STM32F4_TMR_SMCR_SLV_MODE_TRIGGER );
                                        //STM32F4_TMR_SMCR_SLV_MODE_DISABLED );
*/
}

static
int     bsp_ddas_init_adc( void )
{
	//ADC
	RCC->APB2ENR |= ( RCC_APB2ENR_ADC1EN | RCC_APB2ENR_SYSCFGEN );
	__DSB();

	stm32f4_adc_cfg(                ADC1,
	                                STM32F4_ADC_RESOLUTION_12_BIT,
	                                STM32F4_ADC_DATA_ALIGN_RIGHT,
	                                STM32F4_ADC_DMA_MODE_NONSTOP,
	                                STM32F4_ADC_MODE_NONSTOP );

	stm32f4_adc_regular_cfg(        ADC1,
	                                STM32F4_ADC_ANALOG_WATCHDOG_DISABLE,
	                                STM32F4_ADC_SCAN_MULTI,
	                                STM32F4_ADC_REGULAR_ETRG_DISABLE );

	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_1, STM32F4_ADC_CHNL_13 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_2, STM32F4_ADC_CHNL_12 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_3, STM32F4_ADC_CHNL_03 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_4, STM32F4_ADC_CHNL_04 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_5, STM32F4_ADC_CHNL_05 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_6, STM32F4_ADC_CHNL_06 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_7, STM32F4_ADC_CHNL_08 );
	stm32f4_adc_sequence_cfg( ADC1, STM32F4_ADC_SEQUENCE_NUM_8, STM32F4_ADC_CHNL_09 );

	stm32f4_adc_sequence_size_set( ADC1, STM32F4_ADC_SEQUENCE_SIZE_8 );

	stm32f4_adc_enable( ADC1 );

	return( 0 );
}

static
void	bsp_ddas_init_adc_io( void )
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN);
	__DSB();

	stm32f4_gpio_cfg_pin(GPIOC, STM32F4_GPIO_PIN_03, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOC, STM32F4_GPIO_PIN_02, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOA, STM32F4_GPIO_PIN_03, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOA, STM32F4_GPIO_PIN_04, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOA, STM32F4_GPIO_PIN_05, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOA, STM32F4_GPIO_PIN_06, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOB, STM32F4_GPIO_PIN_00, STM32F4_GPIO_CFG_INP_ANALOG);
	stm32f4_gpio_cfg_pin(GPIOB, STM32F4_GPIO_PIN_01, STM32F4_GPIO_CFG_INP_ANALOG);
}

static
void bsp_ddas_init_adc_dma(             const   int16_t *       data,
                                                size_t          size )
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	__DSB();

	stm32f4_dma_disable(            DMA2_Stream0);

	stm32f4_dma_cfg_mem(            DMA2_Stream0,
	                     (uint32_t) data,
	                     (uint32_t) data,
	                                STM32F4_DMA_BURST_NONE,
	                                STM32F4_DMA_XFER_SIZE_16_BIT,
	                                STM32F4_DMA_INCREMENT_ENABLE);

	stm32f4_dma_cfg_peri(           DMA2_Stream0,
	                     (uint32_t) &ADC1->DR,
	                                STM32F4_DMA_BURST_NONE,
	                                STM32F4_DMA_XFER_SIZE_16_BIT,
	                                STM32F4_DMA_INCREMENT_DISABLE);

	stm32f4_dma_cfg_xfer(           DMA2_Stream0,
	                                size,
	                                STM32F4_DMA_RQST_CHNL_0,
	                                STM32F4_DMA_PRIORITY_VERYHIGH,
	                                STM32F4_DMA_CIRCULAR_ENABLE,
	                                STM32F4_DMA_DIR_PERI2MEM);

	stm32f4_dma_irq_cfg(            DMA2_Stream0,
	                                STM32F4_DMA_IRQ_XFER_COMPLETE,
	                                true );

	stm32f4_dma_enable(             DMA2_Stream0);
}

/**
 * @brief BSP DDAS initialization
 */
void bsp_ddas_init(                     const   int16_t *       data,
                                                size_t          size )
{
	bsp_ddas_init_timebase( CFG_DDAS_CHNL_SMPLRATE_SPS * CFG_DDAS_CHNL_MAX );
	bsp_ddas_init_adc_io();
	bsp_ddas_init_adc_dma( data, size );
	bsp_ddas_init_adc();

	//NVIC
	//NVIC_SetPriority(               DMA2_Stream0_IRQn, BSP_NVIC_PRIO_FOC_SENS_U);
	//NVIC_EnableIRQ(                 DMA2_Stream0_IRQn);

	HAL_NVIC_SetPriority(   DMA2_Stream0_IRQn, 13, 0 );
	HAL_NVIC_EnableIRQ(     DMA2_Stream0_IRQn );
}

/**
 * @brief BSP DDAS recieve block
 */
void bsp_ddas_start(                            void )
{
	stm32f4_adc_start( ADC1 );
}

/**
 * @brief BSP DDAS DMA ISR
 */
void bsp_ddas_dma_isr( void )
{
        //HAL_DMA_IRQHandler( hadc.DMA_Handle );
        DMA2->LIFCR	=	DMA_LIFCR_CTCIF0;
}
