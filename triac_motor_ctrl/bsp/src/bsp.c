/**
  * @file bsp.c
  * @brief Board support package for MM2000 HW Rev.A
  * @author Leon Lee (leonlee.scut@gmail.com)
  * @version 0.1
  * @date 2025/08/21
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
  * 	2025/08/21	0.1			Leon Lee		Preliminary version.
  */
#include "bsp.h"
#include "py32f0xx_ll.h"

/**
  * @brief Initialize board support package
  * 
  * @return SUCCESS   - success
  * @return ERROR     - error
  */
int bsp_init(void)
{
    // HAL_Delay(500); // Delay for 500ms to allow power stabilization

    /*
     * Debug Initialize
     *  保证停机模式下调试器继续可以连接使用
     */
#if defined(DEBUG) || defined(_DEBUG)
    // __HAL_DBGMCU_FREEZE_TIM1();
    // __HAL_DBGMCU_FREEZE_WWDG();
    // __HAL_DBGMCU_UNFREEZE_IWDG();
    // __LL_DBGMCU_FREEZE_WWDG();
    // __LL_DBGMCU_FREEZE_IWDG();
#endif
    
    /*
     * GPIO Initialize
     */
    bsp_gpio_init();
    // if (bsp_gpio_init() != 0) return ERROR;
    // if (bsp_encoder_init() != HAL_OK) return HAL_ERROR;

    /*
     * I2C Initialize
     */
    // if (bsp_i2c_init() != HAL_OK) return HAL_ERROR;

    /*
     * ADC Initialize
     */
    
     
    /*
     * Timer Initialize
    //  */
    if (bsp_timer_init() != 0) return ERROR;

    // __HAL_RCC_TIM1_CLK_ENABLE(); // for buzzer
    // if (bsp_buzzer_init(BUZZER_FREQUENCY, BUZZER_DUTY_CYCLE) != HAL_OK) 
    //     return HAL_ERROR;
    bsp_buzzer_init();

    return SUCCESS;
}

void bsp_delay_ms(uint32_t ms)
{
    /*  use nop towait approximately 1 ms  */
    __IO uint32_t i = SystemCoreClock / 1000UL / 4UL / 5UL * ms;

    while (i-- > 0UL)
    {
        __NOP();
    }
}

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
