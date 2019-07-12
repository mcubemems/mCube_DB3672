    
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
 * @file    mcube_tilt_alg.c
 * @author  mCube 
 * @date    22 Mar 2019
 * @brief   main processing of Tilt detection algorithm .
 * @see     http://www.mcubemems.com
 */

#include <stdint.h>
#include <inttypes.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

/*<include files of tilt algo.>*/
#include "mCube_tilt_alg.h"
#include "mCube_tilt_hooks.h"

#define PI (float)3.1415926
#define False (uint8_t)0 
#define True (uint8_t)1  

#define Rad2Deg  (float)(180.0f/PI)


/******************************************************************************
 *** INFORMATION
 *****************************************************************************/
#define M_LIB_TILT_VERSION_ALGORITHM_MAJOR                 1
#define M_LIB_TILT_VERSION_ALGORITHM_MINOR                 0
#define M_LIB_TILT_VERSION_ALGORITHM_BUILD                 0

#define M_LIB_TILT_VERSION_INTERFACE_MAJOR                 1
#define M_LIB_TILT_VERSION_INTERFACE_MINOR                 0
#define M_LIB_TILT_VERSION_INTERFACE_BUILD                 0

/******************************************************************************
 *** MACRO
 *****************************************************************************/
bool Tilt_interface_debug = true;
bool Tilt_alg_debug = false;

#define m_lib_printf(...)   do {                                        \
    if(Tilt_alg_debug)                  \
    mcube_printf(__VA_ARGS__);          \
} while (0);

#define m_lib_interface_printf(...)   do {                                 \
    if(Tilt_interface_debug)               \
    mcube_printf(__VA_ARGS__);             \
} while (0);                            


/******************************************************************************
 ***  mCube Lib version check
 *****************************************************************************/                          
unsigned long mCube_Tilt_GetVersion(void)
{
    return((((uint32_t)M_LIB_TILT_VERSION_ALGORITHM_MAJOR) << 28) |
        (((uint32_t)M_LIB_TILT_VERSION_ALGORITHM_MINOR) << 24) |
        (((uint32_t)M_LIB_TILT_VERSION_ALGORITHM_BUILD) << 20) |
        (((uint32_t)M_LIB_TILT_VERSION_INTERFACE_MAJOR) << 12) |
        (((uint32_t)M_LIB_TILT_VERSION_INTERFACE_MINOR) << 8)  |
        (((uint32_t)M_LIB_TILT_VERSION_INTERFACE_BUILD) << 4));
} 


//=====================================================================================
// Control Paramters
//---------------------------------------------------------------------------------------------------

#define sampleFreq  200.0f      // sample frequency in Hz
#define betaDef     0.35// 2 * proportional gain     //0.095
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;  // quaternion of sensor frame relative to auxiliary frame
float Roll_val, Pitch_val;


//---------------------------------------------------------------------------------------------------
// Output Results
float eul[3]= {0.0f, 0.0f, 0.0f}; 

//---------------------------------------------------------------------------------------------------
// Function declarations

float invSqrt(float x);
void mCubeAhrs_Quat2Eul(float *euler);
void MadgwickIMU_update(float gx, float gy, float gz, float ax, float ay, float az) ;

/******************************************************************************
 ***  mCube Lib enable
 *****************************************************************************/
bool is_Tilt_enable = false;


/**
 *  \brief enable tilt alg
 *  
 *  \return true for success, false for failure
 *  
 *  \details  if is_Tilt_enable=false, set as is_Tilt_enable=true 
 */
bool mCube_Tilt_Open(void)
{

//    m_lib_interface_printf("mCube tilt lib version %x. :    ", mCube_Tilt_GetVersion());

    if(!is_Tilt_enable) 
	{
        is_Tilt_enable = true; 
//        m_lib_interface_printf("tilt_lib Ready. \r\n");
    }

    return is_Tilt_enable;
}


 /**
 *  \brief  check whether Tilt algo is open or not
 *  
 *  \return true is open, false is close
 *  
 */
bool mCube_Tilt_IsOpen(void)
{
    return is_Tilt_enable;
}  


/**
 *  \brief      close and disable algo
 *  
 *  \return     true for success, false for failure
 *  
 *  \details    set "is_Tilt_enable = false"
 */
bool mCube_Tilt_Close(void)
{
    if(is_Tilt_enable)
    {
        is_Tilt_enable = false;
    }
    return is_Tilt_enable;
}

/********************************************************************
 * Coefficents of Butterworth lowpass filter
 **********************************************************************/
#define LP_5Hz_Fs_200Hz

#ifdef LP_5Hz_Fs_200Hz
float a0 = 1.0;
float a1 = -2.68615739655;
float a2 = 2.41965511097;
float a3 = -0.730165345306;
float b0 = 0.000416546139076;
float b1 = 0.00124963841723;
float b2 = 0.00124963841723;
float b3 = 0.000416546139076;
#endif



/**
 *  \brief   low pass filter
 *  
 *  \param [in]   input pointer of sensor data 
 *  
 *  \details    butterworth low pass filter with sample rate = 200Hz and cut-off freq. = 5Hz
 * 
 */
void ButterworthFilter_lowpass_XYZacc (float *input)
{     
    static float xv[3][4]={0.0};
    static float yv[3][4]={0.0};

    uint8_t i=0;
    for(i=0;i<3;i++)
	{
        xv[i][0] = xv[i][1];
        xv[i][1] = xv[i][2];
        xv[i][2] = xv[i][3];
        xv[i][3] = (*(input+i) /1);

        yv[i][0] = yv[i][1];
        yv[i][1] = yv[i][2];
        yv[i][2] = yv[i][3];
        yv[i][3] = ((b0*xv[i][3]+b1*xv[i][2]+b2*xv[i][1]+b3*xv[i][0]-a1*yv[i][2]-a2*yv[i][1]-a3*yv[i][0])/a0);

        *(input+i)=yv[i][3];
    }   
}

/**
 *  \brief interface to recieve data as inputs to algorithm
 *  
 *  \param [in] Acc1_X recieved sensor data 
 *  \param [in] Acc1_Y recieved sensor data  
 *  \param [in] Acc1_Z recieved sensor data 
 *  \return true for success, false for failure
 *  
 *  \details Should be executed in loop by ODR = 200Hz and only support floating data type
 *              and send out tilt angles by "mCube_Tilt_onstatechange"
 */
bool TiltDetection_process(short ax, short ay, short az )
{       
    if(!is_Tilt_enable)
        return false;

    float GyrSensor[3]={0.0f, 0.0f, 0.0f}; 
    float AccSensor[3]={0.0f, 0.0f, 0.0f};

    AccSensor[0] = ax;
	AccSensor[1] = ay;
	AccSensor[2] = az;

    ButterworthFilter_lowpass_XYZacc(AccSensor);
    MadgwickIMU_update(GyrSensor[0], GyrSensor[1], GyrSensor[2], AccSensor[0], AccSensor[1], AccSensor[2]);
    mCubeAhrs_Quat2Eul(eul);
    Roll_val = (float)eul[0]*Rad2Deg;
    Pitch_val = (float)eul[1]*Rad2Deg;
    mCube_Tilt_onstatechange(Roll_val, Pitch_val, AccSensor);
    return true;
}


 /**
 *  \brief IMU algorithm update
 *  
 *  \param [in] gx  value is 0.0f
 *  \param [in] gy  value is 0.0f
 *  \param [in] gz  value is 0.0f
 *  \param [in] ax  recieved accel data with float type
 *  \param [in] ay  recieved accel data with float type
 *  \param [in] az  recieved accel data with float type
 *  
 *  \details   update quaternion, refers to Madgwick fusion algorithm
 *             more details in https://github.com/ZiCog/madgwick.js/blob/master/MadgwickAHRS.c
 *             algorithm only supports floating data type
 */            
void MadgwickIMU_update(float gx, float gy, float gz, float ax, float ay, float az)
{

    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3; 

    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
	{

        // Normalize accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;    

        qDot1 = 0.5f* (-q1 * gx - q2 * gy - q3 * gz);
        qDot2 = 0.5f* ( q0 * gx + q2 * gz - q3 * gy);
        qDot3 = 0.5f* ( q0 * gy - q1 * gz + q3 * gx);
        qDot4 = 0.5f* ( q0 * gz + q1 * gy - q2 * gx);   

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;
        
        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 = qDot1 - betaDef * s0;
        qDot2 = qDot2 - betaDef * s1;
        qDot3=  qDot3 - betaDef * s2;
        qDot4 = qDot4 - betaDef * s3;  
    }

    q0 += qDot1* (1.0f / sampleFreq);
    q1 += qDot2* (1.0f / sampleFreq) ;
    q2 += qDot3* (1.0f / sampleFreq) ;
    q3 += qDot4* (1.0f / sampleFreq) ;

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;  
}


/**
 *  \brief Fast inverse square-root for float point
 *  
 *  \param [in] x input value
 *  \return inverse square-root of input value 
 *  
 *  \details  See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
float invSqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


/**
 *  \brief Bound angle between -180 and 180
 *  
 *  \param [in] dta   angle(rad)
 *  \return           angle(rad)
 *  
 *  \details  here, dta is from atant2(value)
 */
float mCubeAhrs_constrainAngle180(float dta) {
    if(dta >  PI) dta -= (PI*2.0);
    if(dta < -PI) dta += (PI*2.0);
    return dta;
}


/**
 *  \brief Transforms quaternion ([q0,q1,q2,q3]) to euler angles (phi, theta, psi)
 *  
 *  \param [in] euler    these are euler angle(rad)
 *  
 *  \details             euler[2] is yaw angle, 
 *                       it cannot be presented appropiately due to no heading reference 
 */
void mCubeAhrs_Quat2Eul(float *euler)
{
    // roll (x-axis rotation)
    euler[0] = atan2(2*q0*q1+2*q2*q3, 1-2*q1*q1-2*q2*q2); 

    // pitch (y-axis rotation)
    double sinp = 2.0f*(q0*q2-q3*q1);
    if(fabs(sinp) >=0.955f) // default = 1.0f
	{   
        if (sinp < 1e-8)
            euler[1] = -1.0f*PI/2.0f;  // use 90 degrees if out of range                   
        else
            euler[1] = 1.0f*PI/2.0f;    // use 90 degrees if out of range
    }
	else 
	{
        //am_util_stdio_printf(" sinp = %4.6f \r\n",sinp ); 
        euler[1] = asin(sinp);   
    }  

    // yaw (z-axis rotation)
    euler[2] = atan2(2*q0*q3+2*q1*q2, 1-2*q2*q2-2*q3*q3);
    euler[2] = mCubeAhrs_constrainAngle180(euler[2]);
} 


/**
 *  \brief     Get Roll angle(rad)
 *  
 *  \return    Returns the roll angle, rad 
 *  
 */    
float mCubeAhrs_getRoll_rad(void)
{
    return eul[0];
}


/**
 *  \brief     Get Pitch angle(rad)
 *  
 *  \return    Returns the roll angle, rad 
 *  
 */  
float mCubeAhrs_getPitch_rad(void)
{
    return eul[1];
}
