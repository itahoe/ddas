/**
  * @file    app.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "ui.h"
#include "ddas.h"
#include "app.h"
#include "flog.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"


#define STR_SIZEOF              256

	USBD_HandleTypeDef      husbd;
	app_t                   app;
	flog_t                  flog;
static  ddas_smpl_t             data_adc[ CFG_DDAS_BLCK_SIZE_SMPL ][ CFG_DDAS_CHNL_MAX ];

static  ddas_t                  ddas    =     { .data   =   data_adc[0],
                                                .size   =   CFG_DDAS_BLCK_SIZE_SMPL * CFG_DDAS_CHNL_MAX };

static  uint8_t                 str[ STR_SIZEOF ];


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

void HAL_ADC_ConvCpltCallback(          ADC_HandleTypeDef *     hadc )
{
	uint8_t *       data    =   (uint8_t *) data_adc[ CFG_DDAS_BLCK_SIZE_SMPL/2 ];
	size_t          size    =   ( CFG_DDAS_BLCK_SIZE_SMPL/2 ) * 4;

	if( flog.sts.enable )
	{
                snprintf(       str,
                                STR_SIZEOF,
                                "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,\n",
                                data_adc[ 0 ][ 0 ],
                                data_adc[ 0 ][ 1 ],
                                data_adc[ 0 ][ 2 ],
                                data_adc[ 0 ][ 3 ],
                                data_adc[ 0 ][ 4 ],
                                data_adc[ 0 ][ 5 ],
                                data_adc[ 0 ][ 6 ],
                                data_adc[ 0 ][ 7 ] );

                size    =   (4 + 1) * CFG_DDAS_CHNL_MAX + 1;

                ui_led_sd_set( false );
		flog_write( &flog, str, size );
                ui_led_sd_set( true );
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

	ui_init();
	ui_led_sd_set( false );
	ui_led_usb_set( false );

	__enable_irq();

	//HAL_Delay( UI_KEY_LONG_TCKS );

	ui_led_sd_flash(        UI_LED_FLSH_LONG_TCKS );
	ui_led_usb_flash(       UI_LED_FLSH_LONG_TCKS );

	HAL_Delay( CFG_UI_LED_FLSH_LONG_mSEC );

	flog_init( &flog );

	USBD_Init(                      &husbd, &VCP_Desc,      0);
	USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
	USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
	USBD_Start( &husbd );

        ddas_init( &ddas );
        ddas_start( &ddas );

	ui_led_usb_set( true );

	while( true )
	{
		if( app.evt.ui_key_func )
		{
			app.evt.ui_key_func     =    false;

			switch( ui_key_func_status() )
			{
				case UI_KEY_STS_SHORT:
                                        ui_led_usb_flash( UI_LED_FLSH_SHRT_TCKS );
					break;

				case UI_KEY_STS_LONG:
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

				default:
					break;
			}
		}

		if( app.evt.tick_1hz )
		{
			app.evt.tick_1hz    =   false;
		}

/*
		if( app.evt.data0_ready )
		{
			app.evt.data0_ready     =   false;

                        if( flog.sts.enable )
                        {
                                //ui_led_sd_set( false );
                                //flog_write( &flog, gnss_data_uart_rx.data + 0, CFG_FLOG_BLCK_SIZE_OCT/2 );
                                //ui_led_sd_set( true );
                        }
		}

		if( app.evt.data1_ready )
		{
			app.evt.data1_ready     =   false;

                        if( flog.sts.enable )
                        {
                                ui_led_sd_set( false );
                                //flog_write( &flog, gnss_data_uart_rx.data + CFG_FLOG_BLCK_SIZE_OCT/2, CFG_FLOG_BLCK_SIZE_OCT/2 );
                                ui_led_sd_set( true );
                        }
		}
*/

	}
}