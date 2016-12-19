/**
  * @file    bsp_ddas_adc.c
  * @brief   BSP - DDAS ADC services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_ddas.h"


static ADC_HandleTypeDef                hadc;
static TIM_HandleTypeDef                htim;
static DMA_HandleTypeDef                hdma;

static  size_t                          prescaler_tcks  =   0;


/**
  * @brief  DMA transfer complete callback.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static
void ADC_DMAConvCplt(                           DMA_HandleTypeDef *     hdma )
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  /* Update state machine on conversion status if not in error state */
  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA))
  {
    /* Update ADC state machine */
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);

    /* Determine whether any further conversion upcoming on group regular   */
    /* by external trigger, continuous mode or scan sequence on going.      */
    /* Note: On STM32F4, there is no independent flag of end of sequence.   */
    /*       The test of scan sequence on going is done either with scan    */
    /*       sequence disabled or with end of conversion flag set to        */
    /*       of end of sequence.                                            */
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc)                   &&
       (hadc->Init.ContinuousConvMode == DISABLE)            &&
       (HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L) ||
        HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS)  )   )
    {
      /* Disable ADC end of single conversion interrupt on group regular */
      /* Note: Overrun interrupt was enabled with EOC interrupt in          */
      /* HAL_ADC_Start_IT(), but is not disabled here because can be used   */
      /* by overrun IRQ process below.                                      */
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);

      /* Set ADC state */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);

      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }

        /* Conversion complete callback */
        //HAL_ADC_ConvCpltCallback(hadc);
        hal_adc_dma_m0_complete_callback( hadc );
  }
  else
  {
    /* Call DMA error callback */
    hadc->DMA_Handle->XferErrorCallback(hdma);
  }
}

/**
  * @brief  DMA half transfer complete callback.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void ADC_DMAM1ConvCplt(DMA_HandleTypeDef *hdma)
{
        ADC_HandleTypeDef *     hadc    = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
        //HAL_ADC_ConvHalfCpltCallback(hadc);
        hal_adc_dma_m1_complete_callback( hadc );
}

/**
  * @brief  DMA error callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void ADC_DMAError(DMA_HandleTypeDef *hdma)
{
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  hadc->State= HAL_ADC_STATE_ERROR_DMA;
  /* Set ADC error code to DMA error */
  hadc->ErrorCode |= HAL_ADC_ERROR_DMA;
  HAL_ADC_ErrorCallback(hadc);
}

static
void bsp_ddas_adc_timebase_cfg(                 uint32_t                smplrate_sps )
{
        size_t                          apb2_clk_hz     =   HAL_RCC_GetPCLK2Freq();
        size_t                          cnt_period_tcks =   (apb2_clk_hz / (prescaler_tcks + 1)) / smplrate_sps;
        TIM_MasterConfigTypeDef         cfg;



        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_TIM8_FORCE_RESET();
        __HAL_RCC_TIM8_RELEASE_RESET();


        htim.Instance                   =   TIM8;
        htim.Init.Period                =   cnt_period_tcks - 1;
        htim.Init.Prescaler             =   prescaler_tcks;
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
        hadc.Init.ContinuousConvMode    =   DISABLE;
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
        hdma.Init.PeriphDataAlignment   =   DMA_PDATAALIGN_HALFWORD;
        hdma.Init.MemDataAlignment      =   DMA_PDATAALIGN_HALFWORD;
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
void bsp_ddas_adc_start(                        uint16_t *      data_0,
                                                uint16_t *      data_1,
                                                size_t          size )
{
        assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
        assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));
        //__HAL_LOCK( &hadc );

        // Enable the ADC peripheral
        __HAL_ADC_ENABLE( &hadc );

        // Start conversion if ADC is effectively enabled
        if( HAL_IS_BIT_SET( hadc.Instance->CR2, ADC_CR2_ADON ) )
        {
                //Set ADC state
                // - Clear state bitfield related to regular group conversion results
                // - Set state bitfield related to regular group operation
                ADC_STATE_CLR_SET(      hadc.State,
                                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
                                        HAL_ADC_STATE_REG_BUSY );

                // If conversions on group regular are also triggering group injected, update ADC state.
                if( READ_BIT( hadc.Instance->CR1, ADC_CR1_JAUTO ) != RESET )
                {
                        ADC_STATE_CLR_SET( hadc.State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY );
                }

                // State machine update: Check if an injected conversion is ongoing
                if( HAL_IS_BIT_SET( hadc.State, HAL_ADC_STATE_INJ_BUSY ) )
                {
                        //Reset ADC error code fields related to conversions on group regular
                        CLEAR_BIT( hadc.ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA) );
                }
                else
                {
                        //Reset ADC all error code fields
                        ADC_CLEAR_ERRORCODE( &hadc );
                }

                // Process unlocked
                // Unlock before starting ADC conversions: in case of potential interruption, to let the process to ADC IRQ Handler.
                __HAL_UNLOCK( &hadc );

                // Set the DMA transfer complete callback
                hadc.DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;

                // Set the DMA half transfer complete callback
                //hadc.DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
                hadc.DMA_Handle->XferM1CpltCallback     =   ADC_DMAM1ConvCplt;

                // Set the DMA error callback
                hadc.DMA_Handle->XferErrorCallback = ADC_DMAError;


// XferCpltCallback
// XferM1CpltCallback
// XferErrorCallback


                // Manage ADC and DMA start: ADC overrun interruption, DMA start, ADC start (in case of SW start):
                // Clear regular group conversion flag and overrun flag
                // (To ensure of no unknown state from potential previous ADC operations)
                __HAL_ADC_CLEAR_FLAG( &hadc, ADC_FLAG_EOC | ADC_FLAG_OVR );

                // Enable ADC overrun interrupt
                __HAL_ADC_ENABLE_IT( &hadc, ADC_IT_OVR );

                // Enable ADC DMA mode
                hadc.Instance->CR2 |= ADC_CR2_DMA;

                // Start the DMA channel
                //HAL_DMA_Start_IT( hadc.DMA_Handle, (uint32_t) &hadc.Instance->DR, (uint32_t) data_0, size );

                HAL_DMAEx_MultiBufferStart_IT(  hadc.DMA_Handle,
                                                (uint32_t) &hadc.Instance->DR,
                                                (uint32_t) data_0,
                                                (uint32_t) data_1,
                                                size );

                // Check if Multimode enabled
                if( HAL_IS_BIT_CLR( ADC->CCR, ADC_CCR_MULTI ) )
                {
                        // if no external trigger present enable software conversion of regular channels
                        if( ( hadc.Instance->CR2 & ADC_CR2_EXTEN ) == RESET )
                        {
                                // Enable the selected ADC software conversion for regular group
                                hadc.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
                        }
                }
                else
                {
                        // if instance of handle correspond to ADC1 and  no external trigger present enable software conversion of regular channels
                        if( (hadc.Instance == ADC1) && ( (hadc.Instance->CR2 & ADC_CR2_EXTEN) == RESET) )
                        {
                                // Enable the selected ADC software conversion for regular group
                                hadc.Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
                        }
                }
        }

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
