/**
  * @file    bsp_led.h
  * @brief   Board Support Package - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BSP_LED_H
#define  BSP_LED_H


#include	<stdbool.h>


void	bsp_led_gnss_gps_init( void );
bool	bsp_led_gnss_gps_get( void );
void	bsp_led_gnss_gps_set(           const   bool                bval );

void	bsp_led_gnss_dgps_init( void );
bool	bsp_led_gnss_dgps_get( void );
void	bsp_led_gnss_dgps_set(          const   bool                bval );

void	bsp_led_gnss_rtkint_init( void );
bool	bsp_led_gnss_rtkint_get( void );
void	bsp_led_gnss_rtkint_set(        const   bool                bval );

void	bsp_led_gnss_rtkflt_init( void );
bool	bsp_led_gnss_rtkflt_get( void );
void	bsp_led_gnss_rtkflt_set(        const   bool                bval );


void	bsp_led_pwr_r_init( void );
bool	bsp_led_pwr_r_get( void );
void	bsp_led_pwr_r_set(              const   bool                bval );

void	bsp_led_pwr_g_init( void );
bool	bsp_led_pwr_g_get( void );
void	bsp_led_pwr_g_set(              const   bool                bval );

void	bsp_led_pwr_b_init( void );
bool	bsp_led_pwr_b_get( void );
void	bsp_led_pwr_b_set(              const   bool                bval );

void	bsp_led_sd_init( void );
bool	bsp_led_sd_get( void );
void	bsp_led_sd_set(                 const   bool                bval );

void	bsp_led_usb_init( void );
bool	bsp_led_usb_get( void );
void	bsp_led_usb_set(                const   bool                bval );


#endif	//BSP_LED_H
