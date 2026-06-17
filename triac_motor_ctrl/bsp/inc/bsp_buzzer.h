/**
  * @file bsp_buzzer.h
  * @brief Board support package for buzzer control 
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2026/06/16
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
  * 	2026/06/16	0.1			Leon Lee		Preliminary version.
  */

#ifndef __BSP_BUZZER_H__
#define __BSP_BUZZER_H__

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

void bsp_buzzer_init(void);
void bsp_buzzer_on(void);
void bsp_buzzer_off(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // __BSP_BUZZER_H__
/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
  