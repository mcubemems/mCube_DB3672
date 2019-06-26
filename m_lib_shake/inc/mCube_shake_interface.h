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
 * @file    mCube_shake_interface.h
 * @author  mCube
 * @date    15 Apr 2019
 * @brief   Library interface for shake algorithm.
 * @see     http://www.mcubemems.com
 */

#ifndef __M_LIB_SHAKE_INTERFACE_H__
#define __M_LIB_SHAKE_INTERFACE_H__

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
typedef struct
{
    /** Enable debug message, true/false */
    bool s_debug;
    /** Shake threshold, Thrshold = Shaking_STD_THR*10000   */
    /** default: 5 in 100Hz ODR                             */
    uint8_t Shaking_STD_THR;
    /** Shake counts                                        */
    /** default: 5 times in 100Hz ODR                       */
    uint8_t ShakeCounsTHR;
    /** Shake wait period                                   */
    /** default: 2 sec in 100Hz ODR                         */
    uint8_t wait_time;
}mCubeShakeInit_t;

/******************************************************************************
 *** EXTERNAL FUNCTION
 * ODR=100Hz, sensitivity =4g with 14 bit resolution
 *
 *****************************************************************************/
extern bool mCube_Shake_OpenWithParam(mCubeShakeInit_t *initData);
extern bool mCube_Shake_Open(void);
extern bool mCube_Shake_IsOpen(void);
extern bool mCube_Shake_Close(void);
extern bool mCube_Shake_ProcessData(short Acc1_X, short  Acc1_Y, short  Acc1_Z);
extern unsigned long mCube_Shake_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif //__M_LIB_SHAKE_INTERFACE_H__

