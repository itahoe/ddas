/**
  * @file    bsp_ddas_adc.lll.c
  * @brief   BSP - DDAS ADC services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_ddas.h"
#include "stm32f4_lll.h"


static ADC_HandleTypeDef                hadc            =   { .Instance = ADC1 };
static TIM_HandleTypeDef                htim;
static DMA_HandleTypeDef                hdma;

static  size_t                          prescaler_tcks  =   0;


static
void bsp_ddas_adc_timebase_lll_cfg(             uint32_t                smplrate_sps )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  cnt_period_tcks =   (apb2_clk_hz / (prescaler_tcks + 1)) / smplrate_sps;


        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_TIM8_FORCE_RESET();
        __HAL_RCC_TIM8_RELEASE_RESET();

	stm32f4_tmr_cfg(                TIM8,
                                        STM32F4_TMR_CKDIV_1,
                                        STM32F4_TMR_CHNL_DMA_EVT_DEFAULT,
                                        STM32F4_TMR_CHNL_UPD_EVT_COMG_OR_TRGI,
                                        STM32F4_TMR_CHNL_PRELOAD_ENABLE );

	stm32f4_tmr_cnt_cfg(            TIM8,
	                                STM32F4_TMR_CNT_PRELOAD_DISABLE,
	                                STM32F4_TMR_CNT_MODE_UP_NONSTOP,
	                                STM32F4_TMR_CNT_UPD_EVT_SRC_ANY );

        stm32f4_tmr_cfg_master(         TIM8,
                                        STM32F4_TMR_MSTR_OUT_UPDT_EVNT );

	stm32f4_tmr_prescaler_set(      TIM8, prescaler_tcks );

	stm32f4_tmr_period_set(         TIM8, cnt_period_tcks - 1);

	stm32f4_tmr_enable(             TIM8 );

	TIM8->EGR       =   TIM_EGR_COMG | TIM_EGR_UG;
}

static
void bsp_ddas_adc_timebase_hal_cfg(             uint32_t                smplrate_sps )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  cnt_period_tcks =   (apb2_clk_hz / (prescaler_tcks + 1)) / smplrate_sps;


        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_TIM8_FORCE_RESET();
        __HAL_RCC_TIM8_RELEASE_RESET();


        TIM_MasterConfigTypeDef         cfg;

        htim.Instance                   =   TIM8;
        htim.Init.Period                =   cnt_period_tcks - 1;
        htim.Init.Prescaler             =   0;
        htim.Init.ClockDivision         =   0;
        htim.Init.CounterMode           =   TIM_COUNTERMODE_UP;
        htim.Init.RepetitionCounter     =   0;

        if(HAL_TIM_Base_Init(&htim) != HAL_OK)
        {
                //TIM8 Initialization Error
        }

        cfg.MasterOutputTrigger         =   TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             =   TIM_MASTERSLAVEMODE_DISABLE;

        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &cfg ) != HAL_OK )
        {
                //TIM8 TRGO selection Error
        }
}

static
void	bsp_ddas_adc_io_lll_cfg( void )
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN);
	__DSB();

	stm32f4_gpio_pin_cfg( GPIOC, STM32F4_GPIO_PIN_03, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOC, STM32F4_GPIO_PIN_02, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOA, STM32F4_GPIO_PIN_03, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOA, STM32F4_GPIO_PIN_04, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOA, STM32F4_GPIO_PIN_05, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOA, STM32F4_GPIO_PIN_06, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOB, STM32F4_GPIO_PIN_00, STM32F4_GPIO_CFG_INP_ANALOG );
	stm32f4_gpio_pin_cfg( GPIOB, STM32F4_GPIO_PIN_01, STM32F4_GPIO_CFG_INP_ANALOG );
}

static
void bsp_ddas_adc_lll_cfg( void )
{
	RCC->APB2ENR |= ( RCC_APB2ENR_ADC1EN | RCC_APB2ENR_SYSCFGEN );
	__DSB();

	stm32f4_adc_cfg(                ADC1,
	                                STM32F4_ADC_RESOLUTION_12_BIT,
	                                STM32F4_ADC_DATA_ALIGN_RIGHT,
	                                STM32F4_ADC_DMA_MODE_NONSTOP,
	                                STM32F4_ADC_CONVERSION_CONTINIOUS );

	stm32f4_adc_regular_cfg(        ADC1,
	                                STM32F4_ADC_ANALOG_WATCHDOG_DISABLE,
	                                STM32F4_ADC_SCAN_GROUP,
	                                STM32F4_ADC_REGULAR_ETRG_TIM8_TRGO_POSEDGE );

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

}

static
void bsp_ddas_adc_hal_cfg( void )
{
        ADC_ChannelConfTypeDef          cfg1    =   {   .Channel        =   ADC_CHANNEL_13,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg2    =   {   .Channel        =   ADC_CHANNEL_12,
                                                        .Rank           =   2,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg3    =   {   .Channel        =   ADC_CHANNEL_3,
                                                        .Rank           =   3,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg4    =   {   .Channel        =   ADC_CHANNEL_4,
                                                        .Rank           =   4,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg5    =   {   .Channel        =   ADC_CHANNEL_5,
                                                        .Rank           =   5,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg6    =   {   .Channel        =   ADC_CHANNEL_6,
                                                        .Rank           =   6,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg7    =   {   .Channel        =   ADC_CHANNEL_8,
                                                        .Rank           =   7,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };

        ADC_ChannelConfTypeDef          cfg8    =   {   .Channel        =   ADC_CHANNEL_9,
                                                        .Rank           =   8,
                                                        .SamplingTime   =   ADC_SAMPLETIME_3CYCLES,
                                                        .Offset         =   0, };


        //ADC Initialization
        hadc.Instance                   =   ADC1;
        hadc.Init.ClockPrescaler        =   ADC_CLOCKPRESCALER_PCLK_DIV2;
        hadc.Init.Resolution            =   ADC_RESOLUTION_12B;
        hadc.Init.ScanConvMode          =   ENABLE;
        hadc.Init.ContinuousConvMode    =   ENABLE;
        hadc.Init.DiscontinuousConvMode =   DISABLE;
        hadc.Init.NbrOfDiscConversion   =   0;
        hadc.Init.ExternalTrigConvEdge  =   ADC_EXTERNALTRIGCONVEDGE_RISING;
        hadc.Init.ExternalTrigConv      =   ADC_EXTERNALTRIGCONV_T8_TRGO;
        hadc.Init.DataAlign             =   ADC_DATAALIGN_RIGHT;
        hadc.Init.NbrOfConversion       =   8;
        hadc.Init.DMAContinuousRequests =   ENABLE;
        hadc.Init.EOCSelection          =   ADC_EOC_SEQ_CONV;

        if( HAL_ADC_Init( &hadc ) != HAL_OK )
        {
                //ADC Initialization Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg1 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg2 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg3 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg4 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg5 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg6 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg7 ) != HAL_OK )
        {
                //Channel Configuration Error
        }

        if( HAL_ADC_ConfigChannel( &hadc, &cfg8 ) != HAL_OK )
        {
                //Channel Configuration Error
        }
}

static
void bsp_ddas_adc_dma_cfg(              const   uint32_t *      data,
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
void bsp_ddas_adc_init( void )
{
        size_t          smplrate_sps    =   CFG_DDAS_CHNL_SMPLRATE_SPS * CFG_DDAS_CHNL_MAX;

	bsp_ddas_adc_timebase_lll_cfg( smplrate_sps );
	bsp_ddas_adc_io_lll_cfg();
        bsp_ddas_adc_lll_cfg();

	//bsp_ddas_adc_timebase_hal_cfg( smplrate_sps );
	//bsp_ddas_adc_io_hal_cfg();
        //bsp_ddas_adc_hal_cfg();

	HAL_NVIC_SetPriority(   DMA2_Stream0_IRQn, BSP_NVIC_PRIO_DDAS_ADC, 0 );
	HAL_NVIC_EnableIRQ(     DMA2_Stream0_IRQn );

}

/**
 * @brief BSP DDAS recieve block
 */
void bsp_ddas_adc_start(                        uint16_t *              data_0,
                                                uint16_t *              data_1,
                                                size_t                  size )

{
	//bsp_ddas_adc_dma_cfg( data, size );
        //HAL_ADC_Start_DMA( &hadc, data, size );
        //HAL_TIM_Base_Start( &htim );

/*
       //__HAL_ADC_CLEAR_FLAG( &AdcHandle, ADC_FLAG_EOC | ADC_FLAG_OVR);

	stm32f4_adc_enable( ADC1 );
	//stm32f4_adc_start( ADC1 );
        stm32f4_tmr_enable( TIM8 );
*/
}

/**
 * @brief BSP DDAS recieve block
 */
void bsp_ddas_adc_smplrate_set(                 size_t                  smplrate_sps )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  cnt_period_tcks =   (apb2_clk_hz / (prescaler_tcks + 1)) / smplrate_sps;

	stm32f4_tmr_period_set(         TIM8,
                                        cnt_period_tcks - 1 );
}

/**
 * @brief BSP DDAS DMA ISR
 */
void bsp_ddas_adc_dma_isr( void )
{
        //DMA2->LIFCR	=	DMA_LIFCR_CTCIF0;
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}
