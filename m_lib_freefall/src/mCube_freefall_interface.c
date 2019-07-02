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
 * @file    mCube_freefall_interface.c
 * @author  mCube
 * @date    16 Apr 2019
 * @brief   Library interface for freefall algorithm
 * @see     http://www.mcubemems.com
 */

/******************************************************************************
 *** INFORMATION
 *****************************************************************************/
#define M_LIB_FREEFALL_VERSION_ALGORITHM_MAJOR                 1
#define M_LIB_FREEFALL_VERSION_ALGORITHM_MINOR                 0
#define M_LIB_FREEFALL_VERSION_ALGORITHM_BUILD                 1
//freefall lib

#define M_LIB_FREEFALL_VERSION_INTERFACE_MAJOR                 2
#define M_LIB_FREEFALL_VERSION_INTERFACE_MINOR                 0
#define M_LIB_FREEFALL_VERSION_INTERFACE_BUILD                 0

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
/** mCube functions include */
#include "mCube_freefall_alg.h"
#include "mCube_freefall_interface.h"

/******************************************************************************
 *** MACRO
 *****************************************************************************/
bool freefall_interface_debug = 0;
#define m_lib_printf(...)   do {                                            \
    if(freefall_interface_debug)                \
    mcube_printf(__VA_ARGS__);              \
} while (0);

/******************************************************************************
 *** STATIC VARIABLE
 *****************************************************************************/

mCubeFreefallInit_t FreefallParam_Input={0, 0, 0};

bool is_Freefall_enable = false;


 /**
 *  \brief Open free fall algo With customized parameters  
 *  
 *  \param [in] initData customized parameters 
 *  \return true for success, false for failure
 *  
 *  \details all customized parameters can be modified in m_lib_api.c
 */
bool mCube_Freefall_OpenWithParam(mCubeFreefallInit_t *initData)
{
    freefall_interface_debug = initData->s_debug;

    mCube_Freefall_Open();

    if (is_Freefall_enable) 
    {
        FreefallParam_Input.s_debug =
            initData->s_debug;
        FreefallParam_Input.FallCounts_THR =
            initData->FallCounts_THR;
        FreefallParam_Input.FallPower_THR =
            initData->FallPower_THR;

        mCube_Freefall_ParamUpdate(FreefallParam_Input);
        //m_lib_printf("Library configured.");
    }

    return is_Freefall_enable;
}

 /**
 *  \brief Open free fall algo with default parameters 
 *  
 *  \return true for success, false for failure
 *  
 *  \details algo uses default parameters 
 */
bool mCube_Freefall_Open(void)
{
    //m_lib_printf("mCube freefall library version %x. : ", mCube_Freefall_GetVersion());
    if(!is_Freefall_enable)
    {
        is_Freefall_enable = true; 
        //m_lib_printf("Freefall_lib Ready. \r\n");
    }
    return is_Freefall_enable;
}


 /**
 *  \brief  check whether free fall algo is open or close
 *  
 *  \return true is open, false is close
 *  
 *  \details More details
 */
bool mCube_Freefall_IsOpen(void)
{
    return is_Freefall_enable;
}

/**
 *  \brief mCube_Freefall_Close()
 *  
 *  \return true for open, false for close
 *  
 *  \details close algo if it is already open.
 */
bool mCube_Freefall_Close(void)
{
    if(is_Freefall_enable)
    {
        is_Freefall_enable = false;
    }
    return is_Freefall_enable;
}
/**
 *  \brief a interface to recieve data  and input then into algorithm
 *  
 *  \param [in] Acc1_X recieved sensor data
 *  \param [in] Acc1_Y recieved sensor data
 *  \param [in] Acc1_Z recieved sensor data
 *  \return true for success, false for failure
 *  
 *  \details it should be executed in loop by ODR = 200Hz
 */
bool mCube_Freefall_ProcessData(short Acc1_X, short Acc1_Y, short Acc1_Z)
{
    if(!is_Freefall_enable)
        return false;

    FreeFall_ProcessData(Acc1_X, Acc1_Y, Acc1_Z);
    return true;
}

/**
 * @brief Get the version.
 *
 * @return uint32_t Return a 32 bits value it includes
 *         AlGORITHM_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits).
 *         INTERFACE_MAJOR(4bits).MINOR(4bits).BUILD(4bits).Reserve(4bits)
 */
unsigned long mCube_Freefall_GetVersion(void)
{
    return((((uint32_t)M_LIB_FREEFALL_VERSION_ALGORITHM_MAJOR) << 28) |
        (((uint32_t)M_LIB_FREEFALL_VERSION_ALGORITHM_MINOR) << 24) |
        (((uint32_t)M_LIB_FREEFALL_VERSION_ALGORITHM_BUILD) << 20) |
        (((uint32_t)M_LIB_FREEFALL_VERSION_INTERFACE_MAJOR) << 12) |
        (((uint32_t)M_LIB_FREEFALL_VERSION_INTERFACE_MINOR) << 8) |
        (((uint32_t)M_LIB_FREEFALL_VERSION_INTERFACE_BUILD) << 4));
}
