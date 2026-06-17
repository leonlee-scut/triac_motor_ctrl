/**
  * @file pinnames.h
  * @brief IO pins name define head file for PY32F003F18P6TU 
  * @author Leon Lee (leonlee.scut@gmail.com)
  * @version 0.1
  * @date 2023/03/22
  * 
  * @copyright Copyright (c) 2023 South China University of Technology.
  * All rights reserved.
  * 
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  * 
  * @par History:
  * 	Date		Version		Author			Description
  * 	2023/03/22	0.1			Leon Lee		Preliminary version.
  */

#ifndef __PINNAMES_H__
#define __PINNAMES_H__

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

typedef enum {
    PA0        = 0x00,
    PA1        = 0x01,
    PA2        = 0x02,
    PA3        = 0x03,
    PA4        = 0x04,
    PA5        = 0x05,
    PA6        = 0x06,
    PA7        = 0x07,
    // PA8        = 0x08,
    // PA9        = 0x09,
    // PA10       = 0x0A,
    // PA11       = 0x0B,
    PA12       = 0x0C,
    PA13       = 0x0D,
    PA14       = 0x0E,
    // PA15       = 0x0F,
    // PB0        = 0x10,
    // PB1        = 0x11,
    // PB2        = 0x12,
    // PB3        = 0x13,
    // PB4        = 0x14,
    PB5        = 0x15,
    PB6        = 0x16,
    PB7        = 0x17,
    // PB8        = 0x18,
    // PB9        = 0x19,
    // PB10       = 0x1A,
    // PB11       = 0x1B,
    // PB12       = 0x1C,
    // PB13       = 0x1D,
    // PB14       = 0x1E,
    // PB15       = 0x1F,
    // PC13       = 0x2D,
    // PC14       = 0x2E,
    // PC15       = 0x2F,
    // PD0        = 0x30,
    // PD1        = 0x31,

    PF0       = 0x51,
    PF1       = 0x52,
    PF2       = 0x53,
    // PF3      = 0x54,
    PF4       = 0x55,
    /**** ADC internal channels ****/

    // ADC_TEMP = 0xF0, // Internal pin virtual value
    // ADC_VREF = 0xF1, // Internal pin virtual value

    // STDIO for console print

    /**** USB pins ****/
    // USB_DM = PA_11,
    // USB_DP = PA_12,

    /**** OSCILLATOR pins ****/
    // RCC_OSC32_IN = PC_14,
    // RCC_OSC32_OUT = PC_15,
    RCC_OSC_IN = PF0,
    RCC_OSC_OUT = PF1,

    /**** DEBUG pins ****/
    SYS_JTCK_SWCLK = PA14,
    // SYS_JTDI = PA_15,
    // SYS_JTDO_TRACESWO = PB_3,
    SYS_JTMS_SWDIO = PA13,
    // SYS_NJTRST = PB_4,
    // SYS_WKUP = PA_0,

    // Not connected
    // NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif // __PINNAMES_H__

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
