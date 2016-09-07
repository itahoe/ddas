/**
  * @file    bsp_ddas.c
  * @brief   Board Support Package - DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_ddas.h"
#include	"bsp.h"
#include	"config.h"


static	ADC_HandleTypeDef		hadc;
static	DMA_HandleTypeDef               hdma;
static  TIM_HandleTypeDef               htim;


static
int bsp_ddas_init_timer(                        uint32_t        smplrate_sps )
{
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
}

static
int     bsp_ddas_init_adc( void )
{
	int                     resp    =   0;


        hadc.Instance                   =   ADC1;
        hadc.Init.ClockPrescaler        =   ADC_CLOCKPRESCALER_PCLK_DIV2;
        hadc.Init.Resolution            =   ADC_RESOLUTION_12B;
        hadc.Init.ScanConvMode          =   DISABLE;
        hadc.Init.ContinuousConvMode    =   ENABLE;
        hadc.Init.DiscontinuousConvMode =   DISABLE;
        hadc.Init.NbrOfDiscConversion   =   0;
        hadc.Init.ExternalTrigConvEdge  =   ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc.Init.ExternalTrigConv      =   ADC_EXTERNALTRIGCONV_T8_TRGO;
        hadc.Init.DataAlign             =   ADC_DATAALIGN_RIGHT;
        hadc.Init.NbrOfConversion       =   1;
        hadc.Init.DMAContinuousRequests =   ENABLE;
        hadc.Init.EOCSelection          =   DISABLE;

        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_ADC_FORCE_RESET();
        __HAL_RCC_ADC_RELEASE_RESET();

        if( HAL_ADC_Init( &hadc ) != HAL_OK)
        {
                resp    =   -1;
        }

	return( resp );
}

static
void	bsp_ddas_init_adc_io( void )
{
	GPIO_InitTypeDef        gpio_adc01  =   {    .Pin       =    GPIO_PIN_3,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc02  =   {    .Pin       =    GPIO_PIN_2,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc03  =   {    .Pin       =    GPIO_PIN_3,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc04  =   {    .Pin       =    GPIO_PIN_4,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc05  =   {    .Pin       =    GPIO_PIN_5,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc06  =   {    .Pin       =    GPIO_PIN_6,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc07  =   {    .Pin       =    GPIO_PIN_0,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };

	GPIO_InitTypeDef        gpio_adc08  =   {    .Pin       =    GPIO_PIN_1,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL };



	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_adc01 );
	HAL_GPIO_Init( GPIOC, &gpio_adc02 );
	HAL_GPIO_Init( GPIOA, &gpio_adc03 );
	HAL_GPIO_Init( GPIOA, &gpio_adc04 );
	HAL_GPIO_Init( GPIOA, &gpio_adc05 );
	HAL_GPIO_Init( GPIOA, &gpio_adc06 );
	HAL_GPIO_Init( GPIOB, &gpio_adc07 );
	HAL_GPIO_Init( GPIOB, &gpio_adc08 );
}

static
void bsp_ddas_init_adc_dma( void )
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

        //HAL_NVIC_SetPriority( ADCx_DMA_IRQn, 0, 0);   
        //HAL_NVIC_EnableIRQ( ADCx_DMA_IRQn );
}

/**
 * @brief BSP DDAS initialization
 */
void	bsp_ddas_init( void )
{
	bsp_ddas_init_timer( CFG_DDAS_CHNL_SMPLRATE_SPS * CFG_DDAS_CHNL_MAX );
	bsp_ddas_init_adc_io();
	bsp_ddas_init_adc_dma();
	bsp_ddas_init_adc();

	HAL_NVIC_SetPriority(   DMA2_Stream0_IRQn, 13, 0 );
	HAL_NVIC_EnableIRQ(     DMA2_Stream0_IRQn );
}

/**
 * @brief BSP DDAS recieve block
 */
int bsp_ddas_start(                     const   int32_t *       data,
                                                size_t          size )
{
        ADC_ChannelConfTypeDef  adc_cfg_ch1 =   {       .Channel        =   ADC_CHANNEL_1,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch2 =   {       .Channel        =   ADC_CHANNEL_2,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch3 =   {       .Channel        =   ADC_CHANNEL_3,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch4 =   {       .Channel        =   ADC_CHANNEL_4,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch5 =   {       .Channel        =   ADC_CHANNEL_5,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch6 =   {       .Channel        =   ADC_CHANNEL_6,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch7 =   {       .Channel        =   ADC_CHANNEL_7,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };

        ADC_ChannelConfTypeDef  adc_cfg_ch8 =   {       .Channel        =   ADC_CHANNEL_8,
                                                        .Rank           =   1,
                                                        .SamplingTime   =   ADC_SAMPLETIME_28CYCLES,
                                                        .Offset         =   0 };


        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch1 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch2 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch3 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch4 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch5 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch6 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch7 );
        HAL_ADC_ConfigChannel( &hadc, &adc_cfg_ch8 );

        if( HAL_ADC_Start_DMA( &hadc, (uint32_t *) data, size ) != HAL_OK)
        {
                return( -1 ); 
        }

        if( HAL_TIM_Base_Start( &htim ) != HAL_OK)
        {
                return( -1 ); 
        }

        return( 0 );
}

/**
 * @brief BSP DDAS DMA ISR
 */
void bsp_ddas_dma_isr( void )
{
        HAL_DMA_IRQHandler( hadc.DMA_Handle );
}
