/******************************************************************************
*Copyright (C) 2019 mCube, Inc. - All rights reserved
*Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*-Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
*-This software, with or without modification, must only be used with mCube, Inc. hardware.
*-Any software provided in binary form under this license must not be reverse engineered, decompiled, modified and/or disassembled.
*-Neither the name of the copyright holder nor the names of the contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*-Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*
*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*The information provided is believed to be accurate and reliable. The copyright holder assumes no responsibility for the consequences of use of such information nor for any infringement of patents or other rights of third parties which may result from its use. No license is granted by implication or otherwise under any patent rights of the copyright holder.
*****************************************************************************/

/** @defgroup MCUBE_FREEFALL_HOOKS
 *  @
 *  @author mCube
 *  @brief Hook functions for freefall algorithm.
 *  @date 16 Apr 2019
 *  @see http://www.mcubemems.com
 *  @file mCube_freefall_hooks.c
 */

/// Include header files
#include "mCube_FreeFall_hooks.h"
//#include "m_drv_console.h"
//#include "board_main.h"
//#include "ssd1306.h"
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
   /* Implement the hook function here to your application */
    if (newState == true)
    {
        //am_util_stdio_printf("Free Fall Detection!!!!!!!!!!!!! \r\n" ); //Platform dependent HW configuration API
        //am_hal_gpio_out_bit_replace(PIN_LED_G_CTRL, 0x1); //Platform dependent HW configuration API
        sprintf(str_l2, "    Freefall");
    }

}
