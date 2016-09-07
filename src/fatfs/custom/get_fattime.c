/**
  * @file    get_fattime.c
  * @brief   Gets current time.
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <time.h>
#include "diskio.h"

extern	time_t          time_dat;

/**
  * @brief The get_fattime function shall return any valid time even if the system does not support a real time clock.
  * @brief If a zero is returned, the file will not have a valid timestamp.
  * @brief This function is not needed when _FS_READONLY == 1 or _FS_NORTC == 1.
  * @retval Currnet local time is returned with packed into a DWORD value. The bit field is as follows:
  *             bit31:25    Year origin from the 1980 (0..127)
  *             bit24:21    Month (1..12)
  *             bit20:16    Day of the month(1..31)
  *             bit15:11    Hour (0..23)
  *             bit10:5     Minute (0..59)
  *             bit4:0      Second / 2 (0..29) 
  */
DWORD get_fattime( void )
{
	time_t          timer   =   time( NULL );
	struct tm *     p       =   localtime( &timer );
	DWORD           time    =   0;

	//time    =   (( p->tm_year - 1980 ) << 25) |
	time    =   (( p->tm_year - 2000 ) << 25) |
	            (( p->tm_mon + 1)      << 21) |
	            (( p->tm_mday )        << 16) |
	            (( p->tm_hour )        << 11) |
	            (( p->tm_min )         <<  5) |
	            (( p->tm_sec )         >>  1);

	return( time );
}
