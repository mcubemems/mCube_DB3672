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
 * @file    mCube_freefall_alg.c
 * @author  mCube
 * @date    16 Apr 2019
 * @brief   main processing on freefall algorithm
 * @see     http://www.mcubemems.com
 */


#include "mCube_freefall_alg.h"
#include "mCube_freefall_hooks.h"

#define FreeFall_Develper_mode

uint8_t FallCounts_THR =  10 ;
uint16_t FallPower_THR =  375 ;
/******************************************************************************
 *** MACRO
 *****************************************************************************/
bool Freefall_external_debug = false;
bool Freefall_internal_debug = false;

#define m_lib_internal_printf(...)   do {                                    \
    if(Freefall_internal_debug)                  \
    mcube_printf(__VA_ARGS__);               \
} while (0);

#define m_lib_external_printf(...)   do {                                    \
    if(Freefall_external_debug)                  \
    mcube_printf(__VA_ARGS__);               \
} while (0);

/*==========================================================================
  FreeFall_status = 0  : "no event"
  FreeFall_status = 1  : "find potential event of object Fall down"
  FreeFall_status = 2  : "verified Fall down event"
=============================================================================*/
uint8_t FreeFall_status = 0;
bool FallDown_Flag=false;
void mCubeFallDown_Detected(bool FallDown_Flag);
int32_t sqrtI2I( int32_t v ) ;


 
/**
 *  \brief Update Parameters used in algo.
 *  
 *  \param [in] initData desired parameters
 *  
 *  \details 2 parameters associated with algo sensnitivity and 1 debug print mode
 */ 
void mCube_Freefall_ParamUpdate(mCubeFreefallInit_t initData)
{
    Freefall_external_debug = initData.s_debug;
    FallCounts_THR = initData.FallCounts_THR;
    FallPower_THR = initData.FallPower_THR;
    //m_lib_external_printf("===============Get info. of used params in mCube Freefall library============= \r\n");
    //m_lib_external_printf("FallCounts_THR : %d  FallPower_THR = %d \r\n", FallCounts_THR, FallPower_THR);
}

void mCube_FreeFall_initialize(void){
}

/**
 *  \brief Freefall detection
 *  
 *  \param [in] ax accel_ax
 *  \param [in] ay accel_ay
 *  \param [in] az accel_az
 *  \return null
 *  
 *  \details if detect fall_counts>=FallCounts_THR, send out freefall event 
 */ 
void FreeFall_Detection(short ax, short ay, short az){

    static uint8_t fall_counts = 0;
    uint32_t power = sqrtI2I(ax*ax+ay*ay+az*az);

    if(power <= FallPower_THR  && FreeFall_status == 0)
        FreeFall_status = 1;  // detect potential freefall event

    if (FreeFall_status == 1)
    {
        if(power <= FallPower_THR )
        {
            fall_counts += 1;
            m_lib_external_printf("%d th count, power = %d,  \r\n",fall_counts, power );
        }
        else // init all variable used to detect free fall
        {   
            fall_counts = 0;
            FreeFall_status = 0;
            FallDown_Flag = 0;
        }
    }

    if (fall_counts >= FallCounts_THR)
    {
        FallDown_Flag = 1;
        mcube_freefall_onstatechange(FallDown_Flag); // send free fall event flag
    }
}

 /**
 *  \brief freefall algo main code
 *  
 *  \param [in] ax recieved sensor data
 *  \param [in] ay recieved sensor data
 *  \param [in] az recieved sensor data
 *  \return true for success, false for failure
 *  
 *  \details convert sensor data to resolution of 1g = 1024 
 */
bool FreeFall_ProcessData(short ax, short ay, short az)
{
    short AccSensor1[3];
    AccSensor1[0] = ax;
    AccSensor1[1] = ay;
    AccSensor1[2] = az;

    // set sensor data as 1g =1024 count
    FreeFall_Detection(AccSensor1[0]>>1, AccSensor1[1]>>1, AccSensor1[2]>>1);

    return true;
}

/**
 *  \brief fast square root of fixed point
 *  
 *  \param [in] v input value
 *  \return square root of input value
 *  
 *  \details N/A
 */
int32_t sqrtI2I( int32_t v )
{
    uint32_t t, q, b, r;
    r = v;
    b = 0x40000000;
    q = 0;
    while( b > 0 )
    {
        t = q + b;
        q >>= 1;
        if( r >= t )
        {
            r -= t;
            q += b;
        }
        b >>= 2;
    }
    return q;
}
