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

/** @defgroup FREEFALL MCUBE_FREEFALL_HOOKS
 *  @
 *  @author mCube
 *  @brief Hook functions for freefall algorithm.
 *  @date 16 Apr 2019
 *  @see http://www.mcubemems.com
 *  @file m_lib_freefall_hooks.c
 */

/// Include header files
#include "mCube_FreeFall_hooks.h"
#include "m_drv_console.h"
#include "board_main.h"
#include "ssd1306.h"
/**
 ****************************************************************************************
 * @brief mCube freefall state change hook function. It will be called by freefall
 *        if the state of freefall is changed.
 *
 * @param[in] newState      Current state of freefall
 *
 ****************************************************************************************
 */


void mcube_freefall_onstatechange(bool newState)
{
    if (newState == true)
    {
        am_util_stdio_printf("Free Fall Detection!!!!!!!!!!!!! \r\n" );
        am_hal_gpio_out_bit_replace(PIN_LED_G_CTRL, 0x1);
        sprintf(str_l2, "    Freefall");
    }

}
