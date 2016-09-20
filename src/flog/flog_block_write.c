/**
  * @file    flog_block_write.h
  * @brief   File Manager - Name Compose
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<time.h>
#include	"flog.h"


#define STR_SIZEOF              256
static  uint8_t                 str[ STR_SIZEOF ];


int	flog_block_write(                       flog_t *                p,
                                                uint16_t *              data_bin,
                                                size_t                  size_bin,
                                                size_t                  blck_num )
{
        int             resp            =   0;
        size_t          timestamp       =   blck_num * CFG_DDAS_BLOCK_SIZE_SMPL;
        size_t          size            =   (10+1) + (4+1) * CFG_DDAS_CHNL_MAX + 1;


        //size_t i = 1;
        //for( size_t i = 0; i < size_bin; i += CFG_DDAS_CHNL_MAX )
        {
                snprintf(       (char *) str,
                                        STR_SIZEOF,
                                        "%10d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,\n",
                                        timestamp,
/*
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  0 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  1 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  2 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  3 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  4 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  5 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  6 ),
                                        *( data_bin + i*CFG_DDAS_CHNL_MAX +  7 ) );
*/
                                        *( data_bin + 0 ),
                                        *( data_bin + 1 ),
                                        *( data_bin + 2 ),
                                        *( data_bin + 3 ),
                                        *( data_bin + 4 ),
                                        *( data_bin + 5 ),
                                        *( data_bin + 6 ),
                                        *( data_bin + 7 ) );

                flog_write( p, str, size );

                timestamp++;
        }

        return( resp );
}
