/**
 * @file mCube_tilt_hooks.h
 * @author mCube
 * @date 2019/03/22
 * @brief mCube tilt hook functions 
 *  
 */

#ifndef __MCUBE_LIB_TILT_HOOKS_H__
#define __MCUBE_LIB_TILT_HOOKS_H__


#ifdef __cpusplus
extern "C" {
#endif
/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>    
//#include "m_drv_console.h"
/** 
 * -----------------------------------------------------------------------------
 * External linkage variables
 * -----------------------------------------------------------------------------
 */

/** 
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */

void mCube_Tilt_onstatechange(float Roll_val, float Pitch_val, float *AccSensor) ;

#ifdef __cpusplus
}
#endif

#endif /**< !MCUBE_Tilt_HOOKS_H */
