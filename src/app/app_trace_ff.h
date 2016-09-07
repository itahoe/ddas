/**
  * @file    app_trace_ff.h
  * @brief   Trace fatfs
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef APP_TRACE_FF_H
#define APP_TRACE_FF_H


#include <stdio.h>
#include "ff.h"


static
void	app_trace_ff( char * str, int i )
{
	switch( i )
	{
		case FR_OK:                     printf( "%s FR_OK\n",                   str ); break; /* (0) Succeeded */
		case FR_DISK_ERR:               printf( "%s FR_DISK_ERR\n",             str ); break; /* (1) A hard error occurred in the low level disk I/O layer */
		case FR_INT_ERR:                printf( "%s FR_INT_ERR\n",              str ); break; /* (2) Assertion failed */
		case FR_NOT_READY:              printf( "%s FR_NOT_READY\n",            str ); break; /* (3) The physical drive cannot work */
		case FR_NO_FILE:                printf( "%s FR_NO_FILE\n",              str ); break; /* (4) Could not find the file */
		case FR_NO_PATH:                printf( "%s FR_NO_PATH\n",              str ); break; /* (5) Could not find the path */
		case FR_INVALID_NAME:           printf( "%s FR_INVALID_NAME\n",         str ); break; /* (6) The path name format is invalid */
		case FR_DENIED:                 printf( "%s FR_DENIED\n",               str ); break; /* (7) Access denied due to prohibited access or directory full */
		case FR_EXIST:                  printf( "%s FR_EXIST\n",                str ); break; /* (8) Access denied due to prohibited access */
		case FR_INVALID_OBJECT:         printf( "%s FR_INVALID_OBJECT\n",       str ); break; /* (9) The file/directory object is invalid */
		case FR_WRITE_PROTECTED:        printf( "%s FR_WRITE_PROTECTED\n",      str ); break; /* (10) The physical drive is write protected */
		case FR_INVALID_DRIVE:          printf( "%s FR_INVALID_DRIVE\n",        str ); break; /* (11) The logical drive number is invalid */
		case FR_NOT_ENABLED:            printf( "%s FR_NOT_ENABLED\n",          str ); break; /* (12) The volume has no work area */
		case FR_NO_FILESYSTEM:          printf( "%s FR_NO_FILESYSTEM\n",        str ); break; /* (13) There is no valid FAT volume */
		case FR_MKFS_ABORTED:           printf( "%s FR_MKFS_ABORTED\n",         str ); break; /* (14) The f_mkfs() aborted due to any parameter error */
		case FR_TIMEOUT:                printf( "%s FR_TIMEOUT\n",              str ); break; /* (15) Could not get a grant to access the volume within defined period */
		case FR_LOCKED:                 printf( "%s FR_LOCKED\n",               str ); break; /* (16) The operation is rejected according to the file sharing policy */
		case FR_NOT_ENOUGH_CORE:        printf( "%s FR_NOT_ENOUGH_CORE\n",      str ); break; /* (17) LFN working buffer could not be allocated */
		case FR_TOO_MANY_OPEN_FILES:	printf( "%s FR_TOO_MANY_OPEN_FILES\n",  str ); break; /* (18) Number of open files > _FS_SHARE */
		case FR_INVALID_PARAMETER:      printf( "%s FR_INVALID_PARAMETER\n",    str ); break; //(19) Given parameter is invalid
		default:                        printf( "%s FR_UNKNOWN\n",              str ); break;
	}
}

#ifdef  NDEBUG
# define APP_TRACE_FF(...)
#else
# define APP_TRACE_FF( STR, ... )	app_trace_ff( STR, __VA_ARGS__ )
#endif

#endif	//APP_TRACE_FF_H
