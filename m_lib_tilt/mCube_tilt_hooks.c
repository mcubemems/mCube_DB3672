    
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

/**
 * @file    mCube_tilt_hooks.c
 * @author  mCube
 * @date    22 Mar 2019
 * @brief   Tilt angle on pitch and roll
 * @see     http://www.mcubemems.com
 */


#include "mCube_tilt_hooks.h"
//#include "m_drv_console.h"
#include "board_main.h"
#include "ssd1306.h"
#include "mCube_tilt_alg.h"


/**
 *  \brief  print real time tilt angle(deg) on screen
 *  
 *  \param [in] Roll_val  roll angle(rad)
 *  \param [in] Pitch_val pitch angle(rad)
 *  \param [in] AccSensor Acc Sensor data
 *  
 *  \details   Only show roll and pitch on screen and turn on led when angle exceed 30 deg.
 */
 
void mCube_Tilt_onstatechange(float Roll_val, float Pitch_val,float *AccSensor)
{
    char roll_data_direction, pitch_data_direction;
    int roll_data_characteristic = 0;
    int pitch_data_characteristic = 0;

    if (Roll_val > -0.1)
        roll_data_direction = 0x20;
    else
        roll_data_direction = '-';
    
    roll_data_characteristic = (int)(fabs(Roll_val)*10);
    
    if (roll_data_characteristic > 99)
        sprintf(str_l1, " Pitch = %c%d.%d", roll_data_direction, roll_data_characteristic/10, roll_data_characteristic%10);
    else
        sprintf(str_l1, " Pitch = %c%d.%d", roll_data_direction, roll_data_characteristic/10, roll_data_characteristic%10);
	
    if (Pitch_val > -0.1)
        pitch_data_direction = 0x20;
    else
        pitch_data_direction = '-';
    
    pitch_data_characteristic = (int)(fabs(Pitch_val)*10);
    
    if (pitch_data_characteristic > 99)
        sprintf(str_l2, " Roll  = %c%d.%d", pitch_data_direction, pitch_data_characteristic/10, pitch_data_characteristic%10);
    else
        sprintf(str_l2, " Roll  =  %c%d.%d", pitch_data_direction, pitch_data_characteristic/10, pitch_data_characteristic%10);

    if (fabs(Pitch_val) > 30.0f) // Turn on LED when pitch goes over abs(30.0 degreee)
    {
        //am_util_stdio_printf("tilt angle(degree) %4.6f with Y Axis \r\n", Pitch_val ); 
        am_hal_gpio_out_bit_replace(PIN_LED_Y_CTRL, 0x1); //HW GPIO pinout setting to control LED
    }     

    if (fabs(Roll_val) > 30.0f) // Turn on LED when roll goes over abs(30.0 degreee)
    {
        ///am_util_stdio_printf("tilt angle(degree) %4.6f with X Axis \r\n",Roll_val );
        am_hal_gpio_out_bit_replace(PIN_LED_G_CTRL, 0x1);  //HW GPIO pinout setting to control LED    
    } 
}
