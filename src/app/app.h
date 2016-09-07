/**
  * @file    app.h
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "app_trace.h"

typedef	union	app_event_u
{
	uint16_t				whole;
	struct
	{
		bool            tick_1hz        :    1;
		bool            ui_key_pwr      :    1;
		bool            ui_key_func     :    1;
		bool            data0_ready     :    1;
		bool            data1_ready     :    1;
		bool                            :    0;
	};
} app_event_t;

typedef	union	app_status_u
{
	uint16_t				whole;
	struct
	{
		bool            log_enabled     :    1;
		bool            log_expired     :    1;
		bool                            :    0;
	};
} app_status_t;

typedef	struct	app_uart_s
{
	size_t          parity_error;
	size_t          framing_error;
	size_t          noise_error;
	size_t          overrun_error;
	size_t          idle_line;
} app_uart_t;

typedef	struct	app_s
{
	app_event_t     evt;
	app_status_t    sts;
	size_t          tick_1hz;

	#ifndef NDEBUG
	size_t          ism_pckts_sent;
	size_t          tick_1hz_cnt;
	app_uart_t      uart1;
	#endif

}	app_t;


void	app_error( void );

#endif	//APP_H
