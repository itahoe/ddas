/**
  * @file    ui_led.h
  * @brief   User Interface - LED Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  UI_LED_H
#define  UI_LED_H

#include	<stdbool.h>
#include	<string.h>
#include	"config.h"
#include	"bsp.h"

#define	UI_SYSTICK_HZ                   BSP_SYSTICK_HZ

#define	UI_LED_FLSH_SHRT_TCKS           ( ((uint32_t) CFG_UI_LED_FLSH_SHRT_mSEC * UI_SYSTICK_HZ) / 1000 )
#define	UI_LED_FLSH_LONG_TCKS           ( ((uint32_t) CFG_UI_LED_FLSH_LONG_mSEC * UI_SYSTICK_HZ) / 1000 )

/**
  * @brief   Single LED
  */

typedef    void    ( * ui_led_init_t )( void );
typedef    bool    ( * ui_led_get_t )( void );
typedef    void    ( * ui_led_set_t )( bool bval );

typedef	struct	ui_led_s
{
	ui_led_init_t           init;
	ui_led_set_t            set;
	ui_led_get_t            get;
	size_t                  tick;
} ui_led_t;

typedef enum    ui_led_mode_s
{
        UI_LED_MODE_OFF,
        UI_LED_MODE_ON,
        UI_LED_MODE_FORCE,
        UI_LED_MODE_RELEASE,
} ui_led_mode_t;


bool ui_led_hook(                       ui_led_t *              p );

void ui_led_toggle(                     ui_led_t *              p );

void ui_led_set(                        ui_led_t *              p,
                                const   bool                    mode );

void ui_led_flash(                      ui_led_t *              p,
                                        size_t                  len );


bool ui_led_sd_hook(                    void );

void ui_led_sd_toggle(                  void );

void ui_led_sd_set(             const   bool                    mode );

void ui_led_sd_flash(           const   size_t                  len );


bool ui_led_usb_hook(                   void );

void ui_led_usb_toggle(                 void );

void ui_led_usb_set(            const   bool                    mode );

void ui_led_usb_flash(          const   size_t                  len );


#endif	//UI_LED_H
