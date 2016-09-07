/**
  * @file    ui.h
  * @brief   User Interface
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"bsp_key.h"
#include	"bsp_led.h"
#include	"ui.h"


	ui_key_t        ui_key_func             =   {   .init   =   bsp_key_func_init,
	                                                .get    =   bsp_key_func_get,
	                                                .status =   UI_KEY_STS_NONE,
	                                                .tick   =   0 };

	ui_led_t        ui_led_sd               =   {   .init   =   bsp_led_sd_init,
	                                                .get    =   bsp_led_sd_get,
	                                                .set    =   bsp_led_sd_set,
	                                                .tick   =   0 };

	ui_led_t        ui_led_usb              =   {   .init   =   bsp_led_usb_init,
	                                                .get    =   bsp_led_usb_get,
	                                                .set    =   bsp_led_usb_set,
	                                                .tick   =   0 };


/**
 * @brief UI initialization.
 */
void	ui_init( void )
{
	ui_led_sd.init();
	ui_led_usb.init();
	ui_key_func.init();
}
