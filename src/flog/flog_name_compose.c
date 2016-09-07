/**
  * @file    flog_name_compose.h
  * @brief   File Manager - Name Compose
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<time.h>
#include	"flog.h"

int	flog_name_compose(              char *                  str,
	                        const   char *			ext,
	                                size_t                  len_max )
{
	time_t          timer   =   time( NULL );
	struct tm *     curr    =   localtime( &timer );
	int             n       =   0;


	if( curr != NULL )
	{
		n	=   snprintf(   str,
		                        len_max,
		                        "%04d%02d%02d.%02d%02d%02d.%s",
		                        curr->tm_year + 1900,
		                        curr->tm_mon + 1,
		                        curr->tm_mday,
		                        curr->tm_hour,
		                        curr->tm_min,
		                        curr->tm_sec,
		                        ext );
	}

	return( n );
}
