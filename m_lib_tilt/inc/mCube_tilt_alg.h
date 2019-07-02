#ifndef _mCube_TILT_ALG_H_
#define _mCube_TILT_ALG_H_

#ifdef __cplusplus
extern "C" {

#endif
/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
//#include "m_drv_console.h"

extern float Roll_val, Pitch_val;

typedef struct
{
    float yaw ;
    float pitch ;
    float roll ;  
}mCube_EulerAngle_t;

unsigned long mCube_Tilt_GetVersion(void);
bool mCube_Tilt_Open(void);
bool mCube_Tilt_IsOpen(void);
bool mCube_Tilt_Close(void);
bool TiltDetection_process(short ax, short ay, short az);

#endif
