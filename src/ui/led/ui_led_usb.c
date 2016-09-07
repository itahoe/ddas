/**
  * @file    ui_led_usb.c
  * @brief   UI - USB LED.
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

extern	ui_led_t                ui_led_usb;

/**
 * @brief USB LED SysTick Hook.
 */
bool ui_led_usb_hook( void )
{
	bool            resp    =   ui_led_hook( &ui_led_usb );

	return( resp );
}

/**
 * @brief USB LED Toggle.
 */
void ui_led_usb_toggle( void )
{
	ui_led_toggle( &ui_led_usb );
}

/**
 * @brief USB LED Control.
 */
void ui_led_usb_set(            const   bool                    mode )
{
	ui_led_set( &ui_led_usb, mode );
}

/**
 * @brief USB LED Flash.
 */
void ui_led_usb_flash(          const   size_t                  len )
{
	ui_led_flash( &ui_led_usb, len );
}

