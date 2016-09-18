/**
  * @file    app_isr.c
  * @brief   App - Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdint.h>
#include <time.h>
#include "stm32f4xx.h"
#include "app_trace.h"
#include "ui.h"
#include "ff.h"
#include "flog.h"
#include "ddas.h"
#include "app.h"

extern	TIM_HandleTypeDef       htim_cdc;
extern	PCD_HandleTypeDef       hpcd;
extern	app_t                   app;
extern	flog_t                  flog;
	time_t                  time_dat        =   0;


void	hard_fault_handler( uint32_t *arg );
void	NMI_Handler( void );
void	HardFault_Handler( void );
void	MemManage_Handler( void );
void	BusFault_Handler( void );
void	UsageFault_Handler( void );
void	WWDG_IRQHandler( void );
void	SysTick_Handler( void );
void    EXTI0_IRQHandler( void );
void	USART1_IRQHandler( void );
void	DMA2_Stream3_IRQHandler( void );
void	DMA2_Stream6_IRQHandler( void );
void	SDIO_IRQHandler( void );

/**
 * @brief Hard Fault handler
 */
void hard_fault_handler(uint32_t *arg)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	volatile uint32_t stack_ptr;
	volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
	volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;

	stack_ptr = (uint32_t)arg;
	stacked_r0 = arg[0];
	stacked_r1 = arg[1];
	stacked_r2 = arg[2];
	stacked_r3 = arg[3];
	stacked_r12 = arg[4];
	stacked_lr = arg[5];
	stacked_pc = arg[6];
	stacked_psr = arg[7];

	APP_TRACE("\r\n--- Hard fault handler ---\n");
	APP_TRACE("\nstck_ptr\t= 0x%08X", stack_ptr);
	APP_TRACE("\nR0\t= 0x%08X", stacked_r0);
	APP_TRACE("\nR1\t= 0x%08X", stacked_r1);
	APP_TRACE("\nR2\t= 0x%08X", stacked_r2);
	APP_TRACE("\nR3\t= 0x%08X", stacked_r3);
	APP_TRACE("\nR12\t= 0x%08X", stacked_r12);
	APP_TRACE("\nLR[R14]\t= 0x%08X\tsubroutine call return address", stacked_lr);
	APP_TRACE("\nPC[R15]\t= 0x%08X\tprogram counter", stacked_pc);
	APP_TRACE("\nPSR\t= 0x%08X", stacked_psr);
	APP_TRACE("\nBFAR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED38))));
	APP_TRACE("\nCFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED28))));
	APP_TRACE("\nHFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED2C))));
	APP_TRACE("\nDFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED30))));
	APP_TRACE("\nAFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED3C))));
	APP_TRACE("\nSCB_SHCSR\t= 0x%08X", SCB->SHCSR);
	// memstat();
	__asm volatile ("bkpt #0\n");         // Break into the debugger
	while(1);
	#endif //NDEBUG
}

/**
 * @brief NMI interrupt handler
 */
void NMI_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief HardFault interrupt handler
 */
void HardFault_Handler(void)
{
	asm volatile
	(
	        "TST	LR,		#4			\n"
	        "ITE	EQ					\n"
	        "MRSEQ	R0,		MSP			\n"
	        "MRSNE	R0,		PSP			\n"
	        "B		hard_fault_handler"
	);
}

/**
 * @brief Memory Management interrupt handler
 */
void MemManage_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief Bus Fault interrupt handler
 */
void BusFault_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}

	while(1);
	#endif //NDEBUG
}

/**
 * @brief Usage Fault interrupt handler
 */
void UsageFault_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief WindowWatchDog interrupt handler
 */
void WWDG_IRQHandler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief RTC WakeUp interrupt handler
 */
void RTC_WKUP_IRQHandler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief SysTick timer interrupt handler
 */
void SysTick_Handler(void)
{
	ui_led_sd_hook();
	ui_led_usb_hook();
	app.evt.ui_key_func =    ui_key_func_hook() ? true : false;

	if( ++(app.tick_1hz) > BSP_SYSTICK_HZ )
	{
		app.tick_1hz        =    0;
		app.evt.tick_1hz    =    true;

		time_dat++;

		//gnss_time_sync( &gnss, &time_dat );

		#ifndef	NDEBUG
		app.tick_1hz_cnt++;
		#endif
	}

	HAL_IncTick();
}

/**
 * @brief USART1 interrupt handler
 */
void USART1_IRQHandler( void )
{
	//uint32_t        sts                     =   USART1->SR;
	//uint32_t        data                    =   USART1->DR;

        //HAL_UART_IRQHandler( &huart );
}

/**
  * @brief  DMA USART1 TX
  */
void DMA2_Stream7_IRQHandler( void )
{
	//HAL_DMA_IRQHandler( huart.hdmatx );
}

/**
  * @brief  DMA USART1 RX
  */
void DMA2_Stream0_IRQHandler( void )
{
        ddas_adc_dma_isr();
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	BSP_SD_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
	BSP_SD_DMA_Rx_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
	BSP_SD_DMA_Tx_IRQHandler();
}

/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
	HAL_PCD_IRQHandler( &hpcd );
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler( void )
{
	if( __HAL_TIM_GET_FLAG( &htim_cdc, TIM_FLAG_UPDATE ) != RESET )
	{
		if( __HAL_TIM_GET_IT_SOURCE( &htim_cdc, TIM_IT_UPDATE ) !=RESET )
		{
			__HAL_TIM_CLEAR_IT( &htim_cdc, TIM_IT_UPDATE );
			//gnss_uart_rx_hook( &gnss_data_uart_rx );
		}
	}
}
