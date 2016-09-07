/**
  * @file    ui_led_sd.c
  * @brief   UI - SD LED.
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

extern	ui_led_t                ui_led_sd;

/**
 * @brief SD LED SysTick Hook.
 */
bool ui_led_sd_hook( void )
{
	bool            resp    =   ui_led_hook( &ui_led_sd );

	return( resp );
}

/**
 * @brief SD LED Toggle.
 */
void ui_led_sd_toggle( void )
{
	ui_led_toggle( &ui_led_sd );
}

/**
 * @brief SD LED Control.
 */
void ui_led_sd_set(             const   bool                    mode )
{
	ui_led_set( &ui_led_sd, mode );
}

/**
 * @brief SD LED Flash.
 */
void ui_led_sd_flash(           const   size_t                  len )
{
	ui_led_flash( &ui_led_sd, len );
}
