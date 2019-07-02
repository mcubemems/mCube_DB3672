/******************************************************************************
 *
 * Copyright (c) 2019 mCube, Inc. All rights reserved.
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

    if (fabs(AccSensor[0]) > 500/2 && fabs(Pitch_val) > 30.0f) // Turn on LED when pitch goes over abs(30.0 degreee)
    {
        //am_util_stdio_printf("tilt angle(degree) %4.6f with Y Axis \r\n", Pitch_val ); 
        am_hal_gpio_out_bit_replace(PIN_LED_Y_CTRL, 0x1); //HW GPIO pinout setting to control LED
    }     

    if ( fabs(AccSensor[1]) > 500/2 && fabs(Roll_val) > 30.0f) // Turn on LED when roll goes over abs(30.0 degreee)
    {
        ///am_util_stdio_printf("tilt angle(degree) %4.6f with X Axis \r\n",Roll_val );
        am_hal_gpio_out_bit_replace(PIN_LED_G_CTRL, 0x1);  //HW GPIO pinout setting to control LED    
    } 
}
