#ifndef __MCUBE_SHAKING_ALG_H__
#define __MCUBE_SHAKING_ALG_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
//#include "m_drv_console.h"
#include "mCube_shake_interface.h"

bool mCube_Shake_ParamUpdate(mCubeShakeInit_t initData);
bool Shaking_ProcessData(short ax, short ay, short az);

#ifdef __cplusplus
}
#endif
#endif
