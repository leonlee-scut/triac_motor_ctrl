/**
  * @file bsp_adc.h
  * @brief Board support package for ADC control.
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2026/06/29
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
  * 	2026/06/29	0.1			Leon Lee		Preliminary version.
  */

#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

int bsp_adc_init(void);
void bsp_adc_start(void);
void bsp_adc_stop(void);
void bsp_adc_restart(void);

uint16_t *bsp_adc_getRawDataArray(void);
uint16_t bsp_adc_getOversampleRatio(void);
_iq16 bsp_adc_getFilteredData(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // __BSP_ADC_H__
/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
