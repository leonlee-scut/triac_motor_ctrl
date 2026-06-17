/**
  * @file bsp_gpio.c
  * @brief 
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2025/08/21
  * 
  * @copyright Copyright (c) 2025 South China University of Technology.
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


int bsp_gpio_init(void)
{
    uint32_t pin = 0;

    // Enable GPIO clock
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                            LL_IOP_GRP1_PERIPH_GPIOB |
                            LL_IOP_GRP1_PERIPH_GPIOF);

    // Configure GPIO pins for LEDs
    pin = GPIO_PINx(LED_RED);
    LL_GPIO_SetPinMode(GPIOx(LED_RED), pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOx(LED_RED), pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOx(LED_RED), pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(GPIOx(LED_RED), pin, LL_GPIO_SPEED_FREQ_LOW);
    led_off(LED_RED);

    // Configure GPIO pins for FAN control
    pin = GPIO_PINx(FAN_CONTROL);
    LL_GPIO_SetPinMode(GPIOx(FAN_CONTROL), pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOx(FAN_CONTROL), pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOx(FAN_CONTROL), pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(GPIOx(FAN_CONTROL), pin, LL_GPIO_SPEED_FREQ_LOW);
    pinReset(FAN_CONTROL);

    // Configure GPIO pins for motor direction control
    pin = GPIO_PINx(MOTOR_DIR_CONTORL);
    LL_GPIO_SetPinMode(GPIOx(MOTOR_DIR_CONTORL), pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOx(MOTOR_DIR_CONTORL), pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOx(MOTOR_DIR_CONTORL), pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(GPIOx(MOTOR_DIR_CONTORL), pin, LL_GPIO_SPEED_FREQ_LOW);
    pinReset(MOTOR_DIR_CONTORL);

    // Configure GPIO pins for limit switches
    pin = GPIO_PINx(LIMIT_SW_UP);
    LL_GPIO_SetPinMode(GPIOx(LIMIT_SW_UP), pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(LIMIT_SW_UP), pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(LIMIT_SW_DOWN);
    LL_GPIO_SetPinMode(GPIOx(LIMIT_SW_DOWN), pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(LIMIT_SW_DOWN), pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(LIMIT_SW_STATUS);
    LL_GPIO_SetPinMode(GPIOx(LIMIT_SW_STATUS), pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(LIMIT_SW_STATUS), pin, LL_GPIO_PULL_UP);

#if defined(RELEASE) || defined(NDEBUG)
    // Disable debug pins in release mode and set them as FOOT_SW pins
    // __HAL_RCC_DBGMCU_CLK_DISABLE();
    
    // Configure GPIO pins for foot switch inputs (using debug pins in release mode)
    pin = GPIO_PINx(SYS_JTCK_SWCLK);
    LL_GPIO_SetPinMode(GPIOx(SYS_JTCK_SWCLK), pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(SYS_JTCK_SWCLK), pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(SYS_JTMS_SWDIO);
    LL_GPIO_SetPinMode(GPIOx(SYS_JTMS_SWDIO), pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(SYS_JTMS_SWDIO), pin, LL_GPIO_PULL_UP);
#endif // defined(RELEASE) || defined(NDEBUG)


    // Configure GPIO pins for line voltage zero cross detection
    // LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    // pin = GPIO_PINx(LINE_VOLT_ZCD);
    // LL_GPIO_SetPinMode(GPIOx(LINE_VOLT_ZCD), pin, LL_GPIO_MODE_INPUT);
    // LL_GPIO_SetPinPull(GPIOx(LINE_VOLT_ZCD), pin, LL_GPIO_PULL_UP);

    // // Configure EXTI for line voltage zero cross detection (both rising and falling edges)
    // LL_EXTI_SetEXTISource(LINE_VOLT_ZCD_EXTI_CFG_PORT, LINE_VOLT_ZCD_EXTI_CFG_LINE);

    // LL_EXTI_EnableIT(LINE_VOLT_ZCD_EXTI_LINE);
    // LL_EXTI_EnableRisingTrig(LINE_VOLT_ZCD_EXTI_LINE);
    // LL_EXTI_EnableFallingTrig(LINE_VOLT_ZCD_EXTI_LINE);
    // LL_EXTI_ClearFlag(LINE_VOLT_ZCD_EXTI_LINE);

    // NVIC_SetPriority(LINE_VOLT_ZCD_EXTI_IRQn, 2);
    // NVIC_EnableIRQ(LINE_VOLT_ZCD_EXTI_IRQn);

    // Configure GPIO pins for scr gate drive
    // pin = GPIO_PINx(SCR_GATE_DRIVE);
    // LL_GPIO_SetPinMode(GPIOx(SCR_GATE_DRIVE), pin, LL_GPIO_MODE_OUTPUT);
    // LL_GPIO_SetPinOutputType(GPIOx(SCR_GATE_DRIVE), pin, LL_GPIO_OUTPUT_PUSHPULL);
    // LL_GPIO_SetPinPull(GPIOx(SCR_GATE_DRIVE), pin, LL_GPIO_PULL_NO);
    // LL_GPIO_SetPinSpeed(GPIOx(SCR_GATE_DRIVE), pin, LL_GPIO_SPEED_FREQ_LOW);
    // pinReset(SCR_GATE_DRIVE);

    
    return 0;
}


/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
