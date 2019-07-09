#ifndef __MCUBE_FREEFALL_ALG_H__
#define __MCUBE_FREEFALL_ALG_H__

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
//#include "m_drv_console.h"
#include "mCube_freefall_interface.h"

void mCube_Freefall_ParamUpdate(mCubeFreefallInit_t initData);
bool FreeFall_ProcessData(short ax, short ay, short az);

#ifdef __cplusplus
}
#endif
#endif
