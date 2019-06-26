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
 * @file    mCube_shake_alg.c
 * @author  mCube
 * @date    22 Mar 2019
 * @brief   main process for shake algorithm.
 * @see     http://www.mcubemems.com
 */



#include "mCube_shake_alg.h"
#include "mCube_shake_hooks.h"

/******************************************************************************
 *** MACRO
 *****************************************************************************/
bool Shake_internal_debug = false;
bool Shake_external_debug = false;
#define m_lib_internal_printf(...)   do {                                            \
    if(Shake_internal_debug)                  \
    mcube_printf(__VA_ARGS__);              \
} while (0);

#define m_lib_external_printf(...)   do {                                            \
    if(Shake_external_debug)                  \
    mcube_printf(__VA_ARGS__);              \
} while (0);

/*===========================================================================
 *  Set threshold and system parameters
 *=========================================================================*/
#define Shaking_ODR 100 //default :200 Hz

bool  Shaking_trigger = false;
uint32_t Shaking_STD_THR = 50000*(Shaking_ODR/100);  
uint8_t ShakeCounsTHR = 5;                          
uint8_t QuicklyShakeCounts_THR = Shaking_ODR;         
uint32_t Shaking_STD_peak = 50000*(Shaking_ODR/100);
uint32_t Shaking_STD_valley = 50000*(Shaking_ODR/100)/2;
uint8_t wait_time = 2;


 /**
 *  \brief Update customized Parameters used in algo.
 *  
 *  \param [in] initData customized Parameters
 *  \return true for success, false for failure
 *  
 *  \details 5 parameters associated with algo sensnitivity
 */
bool mCube_Shake_ParamUpdate(mCubeShakeInit_t initData)
{
    Shake_external_debug =  initData.s_debug;
    Shaking_STD_THR  =  (uint32_t)initData.Shaking_STD_THR*10000;
    ShakeCounsTHR =  initData.ShakeCounsTHR;        
    wait_time =  initData.wait_time; 
    Shaking_STD_peak = Shaking_STD_THR;                 
    Shaking_STD_valley = Shaking_STD_THR/2; 

    return true;
}

/**
 *  \brief low pass filter
 *  
 *  \param [in] input pointer of sensor data 
 *  
 *  \details butterworth low pass filter with sample rate = 100Hz and cuutoff freq. = 10Hz
 */
void LPF_10Hz_Fs100Hz_Acc (short *input)
{

    static int xv[4][3] = { 0, 0, 0 };
    static int yv[4][3] = { 0, 0, 0 };

    const int32_t a0 = 4096;
    const int32_t a1 = -7232;
    const int32_t a2 = 4880;
    const int32_t a3 = -1155;
    const int32_t b0 = 64;
    const int32_t b1 = 192;
    const int32_t b2 = 192;
    const int32_t b3 = 64;
    uint8_t i=0;

    for (i=0; i<3; i++)
	{

        xv[0][i] = xv[1][i];
        xv[1][i] = xv[2][i];
        xv[2][i] = xv[3][i];
        xv[3][i] = *(input+i) /1;
        yv[0][i] = yv[1][i];
        yv[1][i] = yv[2][i];
        yv[2][i] = yv[3][i];
        yv[3][i] = ((b0*xv[3][i]+b1*xv[2][i]+b2*xv[1][i]+b3*xv[0][i]-a1*yv[2][i]-a2*yv[1][i]-a3*yv[0][i])/a0);
        *(input+i)=yv[3][i];
    }
}

/**
 *  \brief Shaking detection
 *  
 *  \param [in] ax accel_ax
 *  \param [in] ay accel_ay
 *  \param [in] az accel_az
 *  
 *  \details if Shaking_trigger==true, algo send out Shaking event 
 */
void Shaking_Detection3(short ax, short ay, short az)
{
#ifdef Develper_mode_shake
    static int AccumulateCounts = 0;
#endif

    uint8_t i =0;
    short AccSensor1[3];
    int STD = 0;
    static uint8_t Shaking_STD_status = 0;
    static uint8_t QuickCounts = 0;
    static uint8_t peak_counts=0;
    static uint16_t trigger_timer; // 1sec
    static int xyzVariance[3];
    static int xyzMean[3];
    const uint16_t n = 20;    // it approximately measns using 20 samples to esitmate STD.

#ifdef Develper_mode_shake
    static uint8_t valley_counts=0;  // just to debug,it can be removed
#endif

    AccSensor1[0]=ax;
    AccSensor1[1]=ay;
    AccSensor1[2]=az;   //set data as 1g=256 count

    LPF_10Hz_Fs100Hz_Acc(AccSensor1) ;


    for ( i=0; i<3; i++)
	{
        xyzMean[i] = (n-1)*xyzMean[i]/n +  AccSensor1[i]/n;
        xyzVariance[i] = ((n-2)*xyzVariance[i] + n*(AccSensor1[i]-xyzMean[i])*(AccSensor1[i]-xyzMean[i])/(n-1))/(n-1)  ;
    }

    STD = xyzVariance[0] + xyzVariance[1] + xyzVariance[2];

    while (Shaking_STD_status ==1  && STD>Shaking_STD_peak)
    {
        Shaking_STD_peak = STD;
        break;
    }

    while (Shaking_STD_status ==0  && STD<Shaking_STD_valley)
    {
        Shaking_STD_valley = STD;
        break;
    }

    if(STD>=Shaking_STD_THR  && STD>(Shaking_STD_valley*3>>1) && Shaking_trigger==false )
    {
        QuickCounts++ ;
        while (Shaking_STD_status==0){                 
            Shaking_STD_status = 1;                      
            Shaking_STD_valley = Shaking_STD_THR/2 ;    
            QuickCounts = 1;
            trigger_timer = Shaking_ODR*2; 
        }
    }

    if (STD< (Shaking_STD_peak*7)/10 && Shaking_trigger==false && Shaking_STD_status==1)
    {
        Shaking_STD_status = 0;                           
        Shaking_STD_peak = Shaking_STD_THR  ;          
        QuickCounts = 0;
        Shaking_STD_valley=STD;          
        peak_counts++;
        trigger_timer = Shaking_ODR*wait_time; 
    }

    if ( ((peak_counts==ShakeCounsTHR)||(QuickCounts> Shaking_ODR)) && Shaking_trigger==false)
	{    
        Shaking_trigger=true;
        mcube_shake_onstatechange(Shaking_trigger);
        QuickCounts = 0;     // init
        peak_counts = 0;     // init
        Shaking_STD_peak = Shaking_STD_THR;       // init
        Shaking_STD_valley =  Shaking_STD_THR/2;   // init
    }

    if(trigger_timer!=0)
	{
        trigger_timer--;
        if (trigger_timer ==0)
		{
            peak_counts = 0;
            QuickCounts = 0;
            Shaking_trigger=false;
            Shaking_STD_peak = Shaking_STD_THR;
        }
    }
}

/**
 *  \brief Shaking algo main code
 *  
 *  \param [in] ax recieved sensor data
 *  \param [in] ay recieved sensor data
 *  \param [in] az recieved sensor data
 *  \return true for success, false for failure
 *  
 *  \details convert sensor data to resolution of 1g = 1g = 256 count 
 */
bool Shaking_ProcessData(short ax, short ay, short az)
{
    static uint32_t timeindex_shake = 0;

    if( timeindex_shake%1==0)
	{
        ax = ax>>3; ay = ay>>3; az = az>>3;    //  set data as 1g=256 count 
        Shaking_Detection3(ax, ay, az) ;
    }

    timeindex_shake++;

    if (timeindex_shake==65530)
        timeindex_shake = 0;

    return true;
}
