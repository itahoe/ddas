/**
  * @file    ddas.c
  * @brief   DDAS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bsp_ddas.h"
#include "ddas.h"


/**
 * @brief DDAS initialization
 */
void ddas_init(                             ddas_t *            p )
{
	bsp_ddas_init();
}

void ddas_start(                            ddas_t *            p )
{
	bsp_ddas_start( p->data, p->size );
}
