/**
  * @file    app.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "config.h"
#include "ui.h"
#include "ddas.h"
#include "app.h"
#include "flog.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


	USBD_HandleTypeDef      husbd;
	app_t                   app;
	flog_t                  flog;
/*
#pragma pack(4)
static  ddas_smpl_t             data_adc[ 2 ][ CFG_DDAS_BLOCK_SIZE_SMPL * CFG_DDAS_CHNL_MAX ];
#pragma pack()
*/

#pragma pack(4)
static  ddas_pckt_t             ddas_pckt[2]    =
{
        { /*.timestamp = 0,*/ .sync = 0x7E, .size = CFG_DDAS_PCKT_SIZE_OCT >> 8, .spare = 0, },
        { /*.timestamp = 0,*/ .sync = 0x7E, .size = CFG_DDAS_PCKT_SIZE_OCT >> 8, .spare = 0, },
};
#pragma pack()

static  ddas_t                  ddas    =
{
        .adc_smplrate_sps       =   CFG_DDAS_CHNL_SMPLRATE_SPS,
        .data_0                 =   ddas_pckt[0].data,
        .data_1                 =   ddas_pckt[1].data,
};

#pragma pack(4)
static  ddas_pckt_t             ddas_pckt_dummy[2]    =
{
        { /*.timestamp = 0,*/ .sync = 0x7E, .size = CFG_DDAS_PCKT_SIZE_OCT >> 8, .spare = 0, },
        { /*.timestamp = 0,*/ .sync = 0x7E, .size = CFG_DDAS_PCKT_SIZE_OCT >> 8, .spare = 0, },
};
#pragma pack()

static  ddas_t                  ddas_dummy =
{
        .adc_smplrate_sps       =   CFG_DDAS_CHNL_SMPLRATE_SPS,
        .data_0                 =   ddas_pckt_dummy[0].data,
        .data_1                 =   ddas_pckt_dummy[1].data,
};


/**
  * @brief  This function is executed in case of error occurrence.
  */
void	app_error( void )
{
	uint32_t        delay_msec  =   1000;

	while( true )
	{
		ui_led_sd_toggle();
		ui_led_usb_toggle();
		HAL_Delay( delay_msec );
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static
void app_clock_config( void )
{
	RCC_OscInitTypeDef              osc     =   {0};
	RCC_ClkInitTypeDef              clk     =   {0};

        //RCC_OscInitTypeDef      RCC_OscInitStruct;
        //RCC_ClkInitTypeDef      RCC_ClkInitStruct;


        __HAL_RCC_PWR_CLK_ENABLE();

        //The voltage scaling allows optimizing the power consumption when the device is
        //clocked below the maximum system frequency, to update the voltage scaling value
        //regarding system frequency refer to product datasheet.  */
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        osc.OscillatorType      =   RCC_OSCILLATORTYPE_HSE;
        osc.HSEState            =   RCC_HSE_ON;
        osc.PLL.PLLState        =   RCC_PLL_ON;
        osc.PLL.PLLSource       =   RCC_PLLSOURCE_HSE;
        osc.PLL.PLLM            =   16;
        osc.PLL.PLLN            =   336;
        osc.PLL.PLLP            =   2;
        osc.PLL.PLLQ            =   7;
        HAL_RCC_OscConfig( &osc );

        clk.ClockType           =   RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        clk.SYSCLKSource        =   RCC_SYSCLKSOURCE_PLLCLK;
        clk.AHBCLKDivider       =   RCC_SYSCLK_DIV1;
        clk.APB1CLKDivider      =   RCC_HCLK_DIV4;
        clk.APB2CLKDivider      =   RCC_HCLK_DIV2;
        HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_5 );

        //STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported
        if( HAL_GetREVID() == 0x1001 )
        {
                __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
        }
}

static
void adc_dma_complete(                          //ddas_t *                p,
                                                //uint8_t *               data )
                                                ddas_pckt_t *            p )
{
        //uint8_t *         data_flog     =   (uint8_t *) p->data;
        //uint8_t *         data_cdc      =   (uint8_t *) p->data;

        usb_cdc_send( (uint8_t *) p->data, DDAS_PCKT_DATA_SIZE_OCT + DDAS_PCKT_TILE_SIZE_OCT );

	if( flog.sts.enable )
	{
                ui_led_sd_set( false );

                flog_write(     &flog,
                                (uint8_t *) p->data,
                                DDAS_PCKT_DATA_SIZE_OCT );

                ui_led_sd_set( true );
	}
}

void hal_adc_dma_m1_complete_callback(          ADC_HandleTypeDef *     hadc )
{
        //adc_dma_complete( &ddas, (uint8_t *) ddas.data_1 );
        adc_dma_complete( &ddas_pckt[1] );
}

void hal_adc_dma_m0_complete_callback(          ADC_HandleTypeDef *     hadc )
{
        //adc_dma_complete( &ddas, (uint8_t *) ddas.data_0 );
        adc_dma_complete( &ddas_pckt[0] );
}

static
void ddas_test_fill(                            ddas_t *                p )
{
        for( size_t block = 0; block < CFG_DDAS_PCKT_SIZE_OCT/2; block += CFG_DDAS_CHNL_MAX )
        {
                for( size_t ch = 0; ch < CFG_DDAS_CHNL_MAX; ch++ )
                {
                        p->data_0[ block + ch ] = (ch * 512);
                        p->data_1[ block + ch ] = (ch * 512);
                }
        }
}


/**
 * @brief App entry
 */
int main( void )
{
	app_clock_config();
	SysTick_Config(SystemCoreClock / BSP_SYSTICK_HZ);
	HAL_Init();

        APP_TRACE( "DDAS Start\n" );
        APP_TRACE( "CFG_DDAS_PCKT_SIZE_OCT  = %d\n", CFG_DDAS_PCKT_SIZE_OCT);
        //APP_TRACE( "DDAS_PCKT_HEAD_SIZE_OCT = %d\n", DDAS_PCKT_HEAD_SIZE_OCT );
        APP_TRACE( "DDAS_PCKT_DATA_SIZE_OCT = %d\n", DDAS_PCKT_DATA_SIZE_OCT );
        APP_TRACE( "DDAS_PCKT_TILE_SIZE_OCT = %d\n", DDAS_PCKT_TILE_SIZE_OCT );

	ui_init();
	ui_led_sd_set( false );
	ui_led_usb_set( false );

	__enable_irq();

	ui_led_sd_flash(        UI_LED_FLSH_LONG_TCKS );
	ui_led_usb_flash(       UI_LED_FLSH_LONG_TCKS );

	HAL_Delay( CFG_UI_LED_FLSH_LONG_mSEC );

	flog_init( &flog );

	USBD_Init(                      &husbd, &VCP_Desc,      0);
	USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
	USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
	USBD_Start( &husbd );

ddas_test_fill( &ddas );
        //ddas_init( &ddas_dummy );
        //ddas_start( &ddas_dummy );

        ddas_init( &ddas );
        ddas_start( &ddas );
        ddas_vref_enable();

	ui_led_usb_set( true );

	while( true )
	{
		if( app.evt.ui_key_func )
		{
			app.evt.ui_key_func     =    false;

			switch( ui_key_func_status() )
			{
				case UI_KEY_STS_SHORT:
					if( flog.sts.enable )
					{
						flog_close( &flog );
					}
					else
					{
						flog_open( &flog );
					}

					ui_led_sd_set( flog.sts.enable );

					break;

				case UI_KEY_STS_LONG:
                                        if( app.sts.ddas_vref )
                                        {
                                                app.sts.ddas_vref       =   false;
                                                ddas_vref_disable();
                                        }
                                        else
                                        {
                                                app.sts.ddas_vref       =   true;
                                                ddas_vref_enable();
                                        }

                                        //ui_led_usb_set( app.sts.ddas_vref );

					break;

				default:
					break;
			}
		}

		if( app.evt.tick_1hz )
		{
			app.evt.tick_1hz    =   false;
		}
	}
}
