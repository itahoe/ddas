/**
  * @file    bsp_pmu.h
  * @brief   Board Support Package - Power Management Unit
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_PMU_H
#define  BSP_PMU_H

#include	<stdbool.h>

void	bsp_pmu_init( void );

void	bsp_pmu_ldo_enable( bool  bval );

bool	bsp_pmu_sts_charge_get( void );

bool	bsp_pmu_sts_pgood_get( void );

#endif	//BSP_PMU_H
