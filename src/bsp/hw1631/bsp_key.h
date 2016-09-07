/**
  * @file    bsp_key.h
  * @brief   Board Support Package - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_KEY_H
#define  BSP_KEY_H

#include <stdbool.h>

void	bsp_key_func_init( void );

bool	bsp_key_func_get( void );

#endif	//BSP_KEY_H
