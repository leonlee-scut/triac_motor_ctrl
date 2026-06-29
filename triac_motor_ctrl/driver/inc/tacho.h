/**
  * @file tacho.h
  * @brief Header file for tacho driver functions and definitions.
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2026/06/26
  * 
  * @copyright Copyright (c) 2026 South China University of Technology.
  * All rights reserved.
  * 
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  * 
  * @par History:
  * 	Date		Version		Author			Description
  * 	2026/06/26	0.1			Leon Lee		Preliminary version.
  */

#ifndef __TACHO_H__
#define __TACHO_H__

#include "IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

void TACHO_init(uint32_t ref_clk_freq, uint8_t pluse_per_rev);
void TACHO_start(void);
void TACHO_stop(void);
void TACHO_reset(void);
_iq16 TACHO_getSpeedRPM(void);

void TACHO_setRefClkFreq(uint32_t ref_clk_freq);
void TACHO_setPlusePerRev(uint8_t pluse_per_rev);
uint32_t TACHO_getRefClkFreq(void);
uint8_t TACHO_getPlusePerRev(void);

void TACHO_handleCaptureEvent(uint16_t cap_cnt);
void TACHO_handleOverflowEvent(void);

void TACHO_hwInit(void);
void TACHO_hwStart(void);
void TACHO_hwStop(void);
void TACHO_hwReset(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // __TACHO_H__
/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
