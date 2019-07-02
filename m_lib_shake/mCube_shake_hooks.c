
#include "mCube_shake_hooks.h"
//#include "m_drv_console.h"
#include "board_main.h"

/**
 *  \brief Brief description
 *  
 *  \param [in] newState for Shake
 *  \return None
 *  
 *  \details Shake event state change
 */
void mcube_shake_onstatechange(bool newState)
{
    am_hal_gpio_out_bit_replace(PIN_LED_R_CTRL, 0x1); // HW gpio setting to light LEDs. You should link to your HW LED pinout
    sprintf(str_l2, "    Shaken");
}
