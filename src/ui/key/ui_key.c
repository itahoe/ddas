/**
  * @file    ui_key.c
  * @brief   UI - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_key.h"

/**
 * @brief UI - Key hook.
 */
bool	ui_key_hook( ui_key_t *p )
{
	bool            resp            =   false;
	bool            forced          =   p->get();


	if( forced == true )
	{
		p->tick++;

		if( p->tick == UI_KEY_LONG_TCKS )
		{
			p->status       =    UI_KEY_STS_LONG;
			resp            =    true;
		}
	}
	else
	{
		if( p->tick > 0 )
		{
			p->status       =   p->tick < UI_KEY_DEBOUNCE_TCKS ? UI_KEY_STS_NONE :
			                         p->tick < UI_KEY_SHRT_TCKS     ? UI_KEY_STS_SHORT : UI_KEY_STS_RELEASE;

			p->tick		=	0;

			if( p->status != UI_KEY_STS_NONE )
			{
				resp            =    true;
			}
		}
	}

	return( resp );
}
