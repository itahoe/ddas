/**
  * @file    bsp_ddas_adc.c
  * @brief   BSP - DDAS ADC services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"bsp_ddas.h"


static ADC_HandleTypeDef                hadc;
static TIM_HandleTypeDef                htim;
static DMA_HandleTypeDef                hdma;

static  size_t                          prescaler_tcks  =   0;


static
void bsp_ddas_adc_timebase_cfg(                 uint32_t                smplrate_sps )
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
void	bsp_ddas_adc_io_cfg( void )
{
	GPIO_InitTypeDef        gpio_adc_ch1    =   {   .Pin       =    GPIO_PIN_3,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch2    =   {   .Pin       =    GPIO_PIN_2,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch3    =   {   .Pin       =    GPIO_PIN_3,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch4    =   {   .Pin       =    GPIO_PIN_4,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch5    =   {   .Pin       =    GPIO_PIN_5,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch6    =   {   .Pin       =    GPIO_PIN_6,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch7    =   {   .Pin       =    GPIO_PIN_0,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc_ch8    =   {   .Pin       =    GPIO_PIN_1,
	                                                .Mode      =    GPIO_MODE_ANALOG,
	                                                .Pull      =    GPIO_NOPULL };

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_adc_ch1 );
	HAL_GPIO_Init( GPIOC, &gpio_adc_ch2 );
	HAL_GPIO_Init( GPIOA, &gpio_adc_ch3 );
	HAL_GPIO_Init( GPIOA, &gpio_adc_ch4 );
	HAL_GPIO_Init( GPIOA, &gpio_adc_ch5 );
	HAL_GPIO_Init( GPIOA, &gpio_adc_ch6 );
	HAL_GPIO_Init( GPIOB, &gpio_adc_ch7 );
	HAL_GPIO_Init( GPIOB, &gpio_adc_ch8 );
}

static
void bsp_ddas_adc_cfg( void )
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

        __HAL_RCC_ADC1_CLK_ENABLE();

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
        hadc.Init.NbrOfConversion       =   CFG_DDAS_CHNL_MAX;
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
void bsp_ddas_adc_dma_cfg( void )
{
        __HAL_RCC_DMA2_CLK_ENABLE();

        hdma.Instance                   =   DMA2_Stream0;
        hdma.Init.Channel               =   DMA_CHANNEL_0;
        hdma.Init.Direction             =   DMA_PERIPH_TO_MEMORY;
        hdma.Init.PeriphInc             =   DMA_PINC_DISABLE;
        hdma.Init.MemInc                =   DMA_MINC_ENABLE;
        hdma.Init.PeriphDataAlignment   =   DMA_PDATAALIGN_WORD;
        hdma.Init.MemDataAlignment      =   DMA_MDATAALIGN_WORD;
        hdma.Init.Mode                  =   DMA_CIRCULAR;
        hdma.Init.Priority              =   DMA_PRIORITY_HIGH;
        hdma.Init.FIFOMode              =   DMA_FIFOMODE_DISABLE;
        hdma.Init.FIFOThreshold         =   DMA_FIFO_THRESHOLD_HALFFULL;
        hdma.Init.MemBurst              =   DMA_MBURST_SINGLE;
        hdma.Init.PeriphBurst           =   DMA_PBURST_SINGLE;

        HAL_DMA_Init( &hdma );
        __HAL_LINKDMA( &hadc, DMA_Handle, hdma );
}

/**
 * @brief BSP DDAS initialization
 */
void bsp_ddas_adc_init( void )
{
        size_t          smplrate_sps    =   CFG_DDAS_CHNL_SMPLRATE_SPS * CFG_DDAS_CHNL_MAX;

	bsp_ddas_adc_io_cfg();
        bsp_ddas_adc_cfg();
	bsp_ddas_adc_timebase_cfg( smplrate_sps );
        bsp_ddas_adc_dma_cfg();

	HAL_NVIC_SetPriority(   DMA2_Stream0_IRQn, BSP_NVIC_PRIO_DDAS_ADC, 0 );
	HAL_NVIC_EnableIRQ(     DMA2_Stream0_IRQn );
}

/**
 * @brief BSP DDAS recieve block
 */
void bsp_ddas_adc_start(                        uint32_t *      data,
                                                size_t          size )
{
        HAL_ADC_Start_DMA( &hadc, data, size );
        //HAL_ADC_Start_IT( &hadc );
        HAL_TIM_Base_Start( &htim );
}

/**
 * @brief BSP DDAS recieve block
 */
void bsp_ddas_adc_smplrate_set(                 size_t                  smplrate_sps )
{
        size_t                  apb2_clk_hz     =   SystemCoreClock / 2;
        size_t                  cnt_period_tcks =   (apb2_clk_hz / (prescaler_tcks + 1)) / smplrate_sps;

	//stm32f4_tmr_period_set( TIM8, cnt_period_tcks - 1 );
}

/**
 * @brief BSP DDAS DMA ISR
 */
void bsp_ddas_adc_dma_isr( void )
{
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}
