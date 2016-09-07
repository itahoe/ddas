/**
  * @file    ui_key_pwr.c
  * @brief   UI - PWR Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_key.h"

extern	ui_key_t                ui_key_pwr;

/**
 * @brief PWR Key Hook.
 */
bool ui_key_pwr_hook( void )
{
	bool            resp    =   ui_key_hook( &ui_key_pwr );

	return( resp );
}

/**
 * @brief PWR Key Reset.
 */
void ui_key_pwr_reset( void )
{
	ui_key_pwr.status       =   UI_KEY_STS_NONE;
	ui_key_pwr.tick         =   0;
}

/**
 * @brief PWR Key State.
 */
bool ui_key_pwr_forced( void )
{
	bool            resp    =   ui_key_pwr.get();

	return( resp );
}

/**
 * @brief PWR Key Status.
 */
ui_key_sts_t ui_key_pwr_status( void )
{
	return( ui_key_pwr.status );
}
