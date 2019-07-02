/******************************************************************************
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

/**
 * @file    m_lib_freefall_hooks.h
 * @author  mCube
 * @date    16 Apr 2019
 * @brief   Library hooks for freefall algorithm.
 * @see     http://www.mcubemems.com
 */

#ifndef __M_LIB_FREEFALL_HOOKS_H__
#define __M_LIB_FREEFALL_HOOKS_H__

#ifdef __cpusplus
extern "C" {
#endif
/******************************************************************************
 *** HEADER FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/** 
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */

void mcube_freefall_onstatechange(bool newState);

#ifdef __cpusplus
}
#endif

#endif /**< !__M_LIB_FREEFALL_HOOKS_H__ */
