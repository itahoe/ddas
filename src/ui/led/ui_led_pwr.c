/**
  * @file    ui_led_pwr.c
  * @brief   UI - POWER LED's batch (Red-Green-Blue)
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

extern	ui_led_rgb_t            ui_led_pwr;

static
void ui_led_pwr_color_decomp(           bool *                  r,
                                        bool *                  g,
                                        bool *                  b,
                                const   ui_led_rgb_color_t      color )
{
	switch( color )
	{
		case UI_LED_RGB_COLOR_WHITE:    *r = true;  *g = true;  *b = true;  break;
		case UI_LED_RGB_COLOR_MAGENTA:  *r = true;  *g = false; *b = true;  break;
		case UI_LED_RGB_COLOR_CYAN:     *r = false; *g = true;  *b = true;  break;
		case UI_LED_RGB_COLOR_YELLOW:   *r = true;  *g = true;  *b = false; break;
		case UI_LED_RGB_COLOR_BLUE:     *r = false; *g = false; *b = true;  break;
		case UI_LED_RGB_COLOR_GREEN:    *r = false; *g = true;  *b = false; break;
		case UI_LED_RGB_COLOR_RED:      *r = true;  *g = false; *b = false; break;
		case UI_LED_RGB_COLOR_BLACK:    *r = false; *g = false; *b = false; break;
		//case UI_LED_RGB_COLOR_BLACK:    *r = true;  *g = true;  *b = true;  break;
		default:                        *r = false; *g = false; *b = false; break;
	}
}

/**
 * @brief POWER LED SysTick Hook.
 */
void ui_led_pwr_hook( void )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;

	ui_led_hook( &p->r );
	ui_led_hook( &p->g );
	ui_led_hook( &p->b );
}

/**
 * @brief POWER LED Toggle.
 */
void ui_led_pwr_toggle( void )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;
	bool            r, g, b;


	ui_led_pwr_color_decomp( &r, &g, &b, p->color );

	if( r )
	{
		ui_led_toggle( &p->r );
	}

	if( g )
	{
		ui_led_toggle( &p->g );
	}

	if( b )
	{
		ui_led_toggle( &p->b );
	}

}

/**
 * @brief POWER LED Control.
 */
void ui_led_pwr_set(            const   ui_led_rgb_color_t      color )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;
	bool            r, g, b;


	p->color        =   color;

	ui_led_pwr_color_decomp( &r, &g, &b, p->color );

	p->r.set( r );
	p->g.set( g );
	p->b.set( b );
}

/**
 * @brief POWER LED Flash.
 */
void ui_led_pwr_flash(          const   size_t                  len )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;
	bool            r, g, b;


	ui_led_pwr_color_decomp( &r, &g, &b, p->color );

	if( r )
	{
		ui_led_flash( &p->r, len );
	}

	if( g )
	{
		ui_led_flash( &p->g, len );
	}

	if( b )
	{
		ui_led_flash( &p->b, len );
	}
}
