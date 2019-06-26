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
 * @file    mCube_shake_interface.c
 * @author  mCube
 * @date    22 Mar 2019
 * @brief   Library interface for shake algorithm.
 * @see     http://www.mcubemems.com
 */

/******************************************************************************
 *** INFORMATION
 *****************************************************************************/
#define M_LIB_SHAKE_VERSION_AlGORITHM_MAJOR                 1
#define M_LIB_SHAKE_VERSION_AlGORITHM_MINOR                 0
#define M_LIB_SHAKE_VERSION_AlGORITHM_BUILD                 1

#define M_LIB_SHAKE_VERSION_INTERFACE_MAJOR                 2
#define M_LIB_SHAKE_VERSION_INTERFACE_MINOR                 0
#define M_LIB_SHAKE_VERSION_INTERFACE_BUILD                 0

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include "mCube_shake_alg.h"
#include "mCube_shake_interface.h"

/******************************************************************************
 *** MACRO
 *****************************************************************************/
bool shake_interface_debug = 0;
#define m_lib_printf(...)   do {                                            \
    if(shake_interface_debug)                  \
    mcube_printf(__VA_ARGS__);              \
} while (0);

/******************************************************************************
 *** STATIC VARIABLE
 *****************************************************************************/

mCubeShakeInit_t ShakeParam_Input={0, 0, 0, 0};
bool is_Shake_enable = false;

 /**
 *  \brief Open Shake algo with customized parameters  
 *  
 *  \param [in] initData customized parameters 
 *  \return true for success, false for failure
 *  
 *  \details Shake event with customized parameters
 */
bool mCube_Shake_OpenWithParam(mCubeShakeInit_t *initData)
{
    shake_interface_debug = initData->s_debug;

    mCube_Shake_Open();

    if (is_Shake_enable)
	{
        ShakeParam_Input.s_debug =
            initData->s_debug;
        ShakeParam_Input.Shaking_STD_THR =
            initData->Shaking_STD_THR;
        ShakeParam_Input.ShakeCounsTHR=
            initData->ShakeCounsTHR;
        ShakeParam_Input.wait_time=
            initData->wait_time;

        mCube_Shake_ParamUpdate(ShakeParam_Input);
        //m_lib_printf("Library configured.");
    }

    return is_Shake_enable;
}

 /**
 *  \brief Open Shake algo with default parameters 
 *  
 *  \return true for success, false for failure
 *  
 *  \details algo uses default parameters 
 */
bool mCube_Shake_Open(void)
{
    if(!is_Shake_enable) {
        is_Shake_enable = true; 
        //m_lib_printf("Shake_lib Ready. \r\n");
    }
    return is_Shake_enable;
}

 /**
 *  \brief  check whether Shake algo is open or not
 *  
 *  \return true is open, false is close
 *  
 *  \details More details
 */
bool mCube_Shake_IsOpen(void)
{
    return is_Shake_enable;
}

/**
 *  \brief  close algo if it is already open.
 *  
 *  \return true for open, false for close
 *  
 *  \details set "is_Shake_enable = 0"
 */
bool mCube_Shake_Close(void)
{
    if(is_Shake_enable)
    {
        is_Shake_enable = false;
    }
    return is_Shake_enable;
}

/**
 *  \brief a interface to recieve data  and input then into algorithm
 *  
 *  \param [in] Acc1_X recieved sensor data with resolution of 1g = 2048 counts
 *  \param [in] Acc1_Y recieved sensor data with resolution of 1g = 2048 counts 
 *  \param [in] Acc1_Z recieved sensor data with resolution of 1g = 2048 counts
 *  \return true for success, false for failure
 *  
 *  \details it should be executed in loop by ODR = 100Hz 
 */
bool mCube_Shake_ProcessData(short Acc1_X, short Acc1_Y, short Acc1_Z)
{
    if(!is_Shake_enable)
        return false;

    Shaking_ProcessData(Acc1_X, Acc1_Y, Acc1_Z);

    return true;
}

/**
 * @brief Get the version.
 *
 * @return uint32_t Return a 32 bits value it includes
 *         AlGORITHM_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits).
 *         INTERFACE_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits)
 */
unsigned long mCube_Shake_GetVersion(void)
{
    return((((uint32_t)M_LIB_SHAKE_VERSION_AlGORITHM_MAJOR) << 28) |
        (((uint32_t)M_LIB_SHAKE_VERSION_AlGORITHM_MINOR) << 24) |
        (((uint32_t)M_LIB_SHAKE_VERSION_AlGORITHM_BUILD) << 20) |
        (((uint32_t)M_LIB_SHAKE_VERSION_INTERFACE_MAJOR) << 12) |
        (((uint32_t)M_LIB_SHAKE_VERSION_INTERFACE_MINOR) << 8) |
        (((uint32_t)M_LIB_SHAKE_VERSION_INTERFACE_BUILD) << 4));
}
