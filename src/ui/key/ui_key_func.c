/**
  * @file    ui_key_func.c
  * @brief   UI - FUNC Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_key.h"

extern	ui_key_t                ui_key_func;

/**
 * @brief FUNC Key Hook.
 */
bool ui_key_func_hook( void )
{
	bool            resp    =   ui_key_hook( &ui_key_func );

	return( resp );
}

/**
 * @brief FUNC Key Reset.
 */
void ui_key_func_reset( void )
{
	ui_key_func.status      =   UI_KEY_STS_NONE;
	ui_key_func.tick        =   0;
}

/**
 * @brief FUNC Key State.
 */
bool ui_key_func_forced( void )
{
	bool            resp    =   ui_key_func.get();

	return( resp );
}

/**
 * @brief FUNC Key Status.
 */
ui_key_sts_t ui_key_func_status( void )
{
	return( ui_key_func.status );
}
