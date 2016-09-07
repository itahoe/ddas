/**
  * @file    ui.h
  * @brief   User Interface
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  UI_H
#define  UI_H

#include "ui_led.h"
#include "ui_key.h"

typedef struct  ui_s
{
	ui_key_t        key[2];
	ui_led_t        led[3];
} ui_t;

void	ui_init( void );

#endif	//UI_H
