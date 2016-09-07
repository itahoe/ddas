/**
  * @file    flog.h
  * @brief   File Manager
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  FLOG_H
#define  FLOG_H

#include <stdint.h>
#include <stdbool.h>
#include "ff.h"
#include "config.h"

typedef	union	flog_sts_u
{
	uint16_t				whole;
	struct
	{
		bool                    enable          :    1;
		bool                    ready1          :    1;
		bool                    ready2          :    1;
		bool                                    :    0;
	};
} flog_sts_t;

typedef struct  flog_s
{
	size_t          len;
	flog_sts_t      sts;
	FIL             file_log;
	char            fname[ _MAX_LFN ];
	uint8_t *       data;
}	flog_t;

void	flog_init(                      flog_t *                p );

bool	flog_save_hook(                 flog_t *                p,
	                                char                    c );

void	flog_close(                     flog_t *                p );

void	flog_open(                      flog_t *                p );

void	flog_write(                     flog_t *                p,
	                        const   uint8_t *               data,
	                                size_t                  len );

int	flog_name_compose(              char *                  str,
	                        const   char *			ext,
	                                size_t                  len_max );


#endif	//FLOG_H
