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
 * @file    m_lib_freefall_interface.h
 * @author  mCube
 * @date    16 Apr 2019
 * @brief   Library interface for freefall algorithm.
 * @see     http://www.mcubemems.com
 */

#ifndef __M_LIB_FREEFALL_INTERFACE_H__
#define __M_LIB_FREEFALL_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "m_drv_console.h"

/******************************************************************************
 *** DATA TYPE / STRUCTURE DEFINITION / ENUM
 *****************************************************************************/
/** Library configuration. */
typedef struct
{
    /** Enable debug message, true/false */
    bool s_debug;
    /** Freefall counts                                      */
    /** default: 10 in 200Hz ODR                             */
    uint8_t FallCounts_THR;
    /** Freefall threshold                                   */
    /** default: 375 in 200Hz ODR                            */
    uint16_t FallPower_THR ;
}mCubeFreefallInit_t;

/******************************************************************************
 *** EXTERNAL FUNCTION
 *** ODR=200Hz, sensitivity =4g with 14 bit resolution, 1g = 2048 cnts
 *****************************************************************************/
extern bool mCube_Freefall_OpenWithParam(mCubeFreefallInit_t *initData);
extern bool mCube_Freefall_Open(void);
extern bool mCube_Freefall_IsOpen(void);
extern bool mCube_Freefall_Close(void);
extern bool mCube_Freefall_ProcessData(short Acc1_X, short  Acc1_Y, short  Acc1_Z);
extern unsigned long mCube_Freefall_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif //__M_LIB_FREEFALL_INTERFACE_H__

