/**
  * @file bsp_buzzer.c
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

#include "bsp.h"
#include "py32f0xx_ll.h"
#include "bsp_buzzer.h"

/**
  * @note Use MCO output as buzzer clock source, which is configured to output 
  *       LSI divided by 8 (4096Hz). This allows us to save one timer peripheral
  *       for ADC oversampling use.
  */

void bsp_buzzer_init(void)
{
    GPIO_TypeDef *port = GPIOx(BUZZER_CLK_PIN);
    uint32_t pin = GPIO_PINx(BUZZER_CLK_PIN);

    // Configure GPIO pins for BUZZER clock output (MCO)
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_ResetOutputPin(port, pin);

    // Enable LSI and wait for it to be ready
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1) {
        // Optionally add a timeout here to avoid an infinite loop
    }

    // Configure MCO to output LSI divided by 8
    LL_RCC_ConfigMCO(BUZZER_CLK_SOURCE, BUZZER_CLK_DIV);
    // LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK, LL_RCC_MCO1_DIV_1);
}


void bsp_buzzer_on(void)
{
    GPIO_TypeDef *port = GPIOx(BUZZER_CLK_PIN);
    uint32_t pin = GPIO_PINx(BUZZER_CLK_PIN);

    // Configure Buzzer pin as alternate function for MCO output
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(port, pin, BUZZER_CLK_PIN_AF);
}


void bsp_buzzer_off(void)
{
    GPIO_TypeDef *port = GPIOx(BUZZER_CLK_PIN);
    uint32_t pin = GPIO_PINx(BUZZER_CLK_PIN);

    // Configure Buzzer pin as general purpose output and set it low
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(port, pin);
}

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
