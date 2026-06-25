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
#include "bsp_gpio.h"


int bsp_gpio_init(void)
{
    uint32_t pin = 0;
    GPIO_TypeDef *port = NULL;

    // Enable GPIO clock
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                            LL_IOP_GRP1_PERIPH_GPIOB |
                            LL_IOP_GRP1_PERIPH_GPIOF);

    // Configure GPIO pins for LEDs
    pin = GPIO_PINx(LED_RED);
    port = GPIOx(LED_RED);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
    led_off(LED_RED);

    // Configure GPIO pins for FAN control
    pin = GPIO_PINx(FAN_CONTROL);
    port = GPIOx(FAN_CONTROL);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
    pinReset(FAN_CONTROL);

    // Configure GPIO pins for motor direction control
    pin = GPIO_PINx(MOTOR_DIR_CONTORL);
    port = GPIOx(MOTOR_DIR_CONTORL);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
    pinReset(MOTOR_DIR_CONTORL);

    // Configure GPIO pins for limit switches
    pin = GPIO_PINx(LIMIT_SW_UP);
    port = GPIOx(LIMIT_SW_UP);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(LIMIT_SW_DOWN);
    port = GPIOx(LIMIT_SW_DOWN);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(LIMIT_SW_STATUS);
    port = GPIOx(LIMIT_SW_STATUS);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

#if defined(RELEASE) || defined(NDEBUG)
    // Disable debug pins in release mode and set them as FOOT_SW pins
    // __HAL_RCC_DBGMCU_CLK_DISABLE();
    
    // Configure GPIO pins for foot switch inputs (using debug pins in release mode)
    pin = GPIO_PINx(SYS_JTCK_SWCLK);
    port = GPIOx(SYS_JTCK_SWCLK);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(SYS_JTMS_SWDIO);
    port = GPIOx(SYS_JTMS_SWDIO);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);
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

    // Configure GPIO pins for tacho input
    pin = GPIO_PINx(TACHO_INPUT);
    port = GPIOx(TACHO_INPUT);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    // Configure GPIO pins for encoder input
    pin = GPIO_PINx(ENCODER_A);
    port = GPIOx(ENCODER_A);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    pin = GPIO_PINx(ENCODER_B);
    port = GPIOx(ENCODER_B);
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);

    return 0;
}


void bsp_tacho_exti_enable(void)
{
    LL_EXTI_SetEXTISource(TACHO_INPUT_EXTI_CFG_PORT, TACHO_INPUT_EXTI_CFG_LINE);
    LL_EXTI_EnableFallingTrig(TACHO_INPUT_EXTI_LINE);
    
    CRITICAL_SECTION_ENTER();

    LL_EXTI_ClearFlag(TACHO_INPUT_EXTI_LINE);
    LL_EXTI_EnableIT(TACHO_INPUT_EXTI_LINE);

    if (NVIC_GetEnableIRQ(TACHO_INPUT_EXTI_IRQn) == DISABLE)
    {
        NVIC_ClearPendingIRQ(TACHO_INPUT_EXTI_IRQn);
        NVIC_SetPriority(TACHO_INPUT_EXTI_IRQn, TACHO_INPUT_IRQ_PRIORITY);
        NVIC_EnableIRQ(TACHO_INPUT_EXTI_IRQn);
    }

    CRITICAL_SECTION_EXIT();

}

void bsp_tacho_exti_disable(void)
{
    CRITICAL_SECTION_ENTER();

    LL_EXTI_DisableIT(TACHO_INPUT_EXTI_LINE);
    // NVIC_DisableIRQ(TACHO_INPUT_EXTI_IRQn);

    CRITICAL_SECTION_EXIT();
}

void bsp_encoder_exti_enable(void)
{

    LL_EXTI_SetEXTISource(ENCODER_A_EXTI_CFG_PORT, ENCODER_A_EXTI_CFG_LINE);
    LL_EXTI_EnableRisingTrig(ENCODER_A_EXTI_LINE);
    
    LL_EXTI_SetEXTISource(ENCODER_B_EXTI_CFG_PORT, ENCODER_B_EXTI_CFG_LINE);
    LL_EXTI_EnableRisingTrig(ENCODER_B_EXTI_LINE);
    
    CRITICAL_SECTION_ENTER();

    LL_EXTI_ClearFlag(ENCODER_A_EXTI_LINE);
    LL_EXTI_ClearFlag(ENCODER_B_EXTI_LINE);
    LL_EXTI_EnableIT(ENCODER_B_EXTI_LINE);
    LL_EXTI_EnableIT(ENCODER_A_EXTI_LINE);

    if (NVIC_GetEnableIRQ(ENCODER_A_EXTI_IRQn) == DISABLE)
    {
        NVIC_ClearPendingIRQ(ENCODER_A_EXTI_IRQn);
        NVIC_SetPriority(ENCODER_A_EXTI_IRQn, ENCODER_IRQ_PRIORITY);
        NVIC_EnableIRQ(ENCODER_A_EXTI_IRQn);
    }

    if (NVIC_GetEnableIRQ(ENCODER_B_EXTI_IRQn) == DISABLE)
    {
        NVIC_ClearPendingIRQ(ENCODER_B_EXTI_IRQn);
        NVIC_SetPriority(ENCODER_B_EXTI_IRQn, ENCODER_IRQ_PRIORITY);
        NVIC_EnableIRQ(ENCODER_B_EXTI_IRQn);
    }

    CRITICAL_SECTION_EXIT();
}

void bsp_encoder_exti_disable(void)
{
    CRITICAL_SECTION_ENTER();

    LL_EXTI_DisableIT(ENCODER_A_EXTI_LINE);
    LL_EXTI_DisableIT(ENCODER_B_EXTI_LINE);
    // NVIC_DisableIRQ(ENCODER_A_EXTI_IRQn);
    // NVIC_DisableIRQ(ENCODER_B_EXTI_IRQn);

    CRITICAL_SECTION_EXIT();
}


__WEAK void TACHO_IN_TriggerCallback(uint32_t capture_count)
{
    // User-defined callback function for TACHO input trigger event
    // This function can be overridden in the user application
}

void EXTI2_3_IRQHandler(void)
{
    // Handle ENCODER phase B input trigger event
    if (LL_EXTI_IsActiveFlag(ENCODER_B_EXTI_LINE))
    {
        LL_EXTI_ClearFlag(ENCODER_B_EXTI_LINE);
        // Handle ENCODER B input trigger event
        // User-defined callback function can be called here
    }
}
void EXTI4_15_IRQHandler(void)
{
    // Handle ENCODER phase A input trigger event
    if (LL_EXTI_IsActiveFlag(ENCODER_A_EXTI_LINE))
    {
        LL_EXTI_ClearFlag(ENCODER_A_EXTI_LINE);
        // Handle ENCODER A input trigger event
        // User-defined callback function can be called here
    }

    // Handle TACHO input trigger event
    if (LL_EXTI_IsActiveFlag(TACHO_INPUT_EXTI_LINE))
    {
        LL_EXTI_ClearFlag(TACHO_INPUT_EXTI_LINE);

        // Handle TACHO input trigger event
        /* Get the current counter value of the TACHO timer */
        uint32_t capture_count = LL_TIM_GetCounter(TACHO_TIM_INSTANCE); 
        TACHO_IN_TriggerCallback(capture_count);
    }
}

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
