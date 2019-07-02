/*****************************************************************************
 *
 * Copyright (c) 2019 mCube, Inc.  All rights reserved.
 *
 * This source is subject to the mCube Software License.
 * This software is protected by Copyright and the information and source code
 * contained herein is confidential. The software including the source code
 * may not be copied and the information contained herein may not be used or
 * disclosed except with the written permission of mCube Inc.
 *
 * All other rights reserved.
 *****************************************************************************/

/** @defgroup SHAKE MCUBE_SHAKE_HOOKS
 *  @author mCube
 *  @brief Hook functions for shake algorithm
 *  @date 16 Apr 2019
 *  @see http://www.mcubemems.com
 *  @file mCube_shake_hooks.h
 */

#ifndef __MCUBE_LIB_SHAKE_HOOKS_H__
#define __MCUBE_LIB_SHAKE_HOOKS_H__

#ifdef __cpusplus
extern "C" {
#endif
/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
//#include "m_drv_console.h"
/** 
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */

void mcube_shake_onstatechange(bool newState);

#ifdef __cpusplus
}
#endif

#endif /**< !MCUBE_SHAKING_HOOKS_H */
