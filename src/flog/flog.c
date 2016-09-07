/**
  * @file    flog.h
  * @brief   File Manager
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<time.h>
#include	"bsp_flog.h"
#include	"flog.h"
#include	"diskio.h"
#include	"ff.h"
#include	"app.h"

FATFS   SDFatFs;  /* File system object for SD card logical drive */
char    SDPath[4]; /* SD card logical drive path */


/**
 * @brief File Manager initialization
 */
void	flog_init(                      flog_t *                p )
{

}

/**
 * @brief File Manager file close
 */
void	flog_close(                     flog_t *                p )
{
	FRESULT         resp;


        p->sts.enable   =   false;

	resp            =   f_close( &p->file_log );
	APP_TRACE_FF( "f_close() = ", resp );

        if( resp == FR_OK )
        {
                p->sts.enable   =   false;
        }
        else
        {
                p->sts.enable   =   false;
        }
}

/**
 * @brief File Manager file open
 */
void	flog_open(                      flog_t *                p )
{
	FRESULT         resp;


	flog_name_compose( p->fname, "csv", sizeof( p->fname ) );

	resp    =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);
	APP_TRACE_FF( "f_open() = ", resp );

	if( resp == FR_OK )
        {
                p->sts.enable   =   true;
        }
        else
	{
		resp    =   f_mount( NULL, (TCHAR const*) SDPath, 1 );
                APP_TRACE_FF( "f_mount() = ", resp );

		resp    =   f_mount( &SDFatFs, (TCHAR const*) SDPath, 1 );
                APP_TRACE_FF( "f_mount(SD) = ", resp );

		if( resp == FR_OK )
		{
			resp            =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);

                        APP_TRACE_FF( "f_open() = ", resp );

                        p->sts.enable   =   (resp == FR_OK) ? true : false;
		}
                else
                {
                        p->sts.enable   =   false;
                }
	}
}

/**
 * @brief File Manager write
 */
void	flog_write(                     flog_t *                p,
	                        const   uint8_t *               pbuf,
	                                size_t                  len )
{
	FRESULT         resp;
	uint32_t        wr_bytes        =   0;


        while( len > 0 )
        {
                resp    =   f_write( &p->file_log, pbuf, len, (void *) &wr_bytes );

                if( resp == FR_OK )
                {
                        if( wr_bytes < len )
                        {
                                APP_TRACE( "f_write() - len: %d, wr_bytes: %d\n", len, wr_bytes );
                        }
                        else
                        {
                                //f_sync( &p->file_log );
                        }

                        len     -=  wr_bytes;
                        pbuf    +=  wr_bytes;
                }
                else
                {
                        APP_TRACE_FF( "f_write() = ", resp );
                        APP_TRACE( "pbuf: %0X\n", pbuf );
                        APP_TRACE( "len: %d\n", len );
                        APP_TRACE( "wr_bytes: %d\n", wr_bytes );
                        break;
                }
        }

        if( resp == FR_OK )
        {
                //f_sync( &p->file_log );
        }
}
