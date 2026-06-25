/**
  * @file bsp_gpio.h
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

#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmsis_compiler.h"
#include "RTE_Components.h"
#include CMSIS_device_header
#include "py32f0xx_ll_gpio.h"
#include "PinNames.h"
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

#if !defined(GPIO_PIN_SET)
#define GPIO_PIN_SET            0b1
#endif

#if !defined(GPIO_PIN_RESET)
#define GPIO_PIN_RESET          0b0
#endif

/**
  * @brief Get GPIO port from PinName
  * 
  * @param pin Pin name of a GPIO pin 
  * @return GPIO_TypeDef* GPIO Port
  */
__STATIC_INLINE GPIO_TypeDef *GPIOx(PinName pin)
{
    uint32_t port_reg_size = 0x0400;
    uint32_t portNmu = (pin & 0xF0) >> 4;

    return (GPIO_TypeDef *)(GPIOA_BASE + port_reg_size * portNmu);
}

/**
  * @brief Get pin number from PinName
  * 
  * @param pin Pin name of a GPIO pin 
  * @return int 
  */
__STATIC_INLINE uint8_t pinNum(PinName pin)
{
    return pin & 0x0F;
}

/**
  * @brief Return a bit mask for a given PinName
  * 
  * @param pin Pin name of the GPIO pin
  * @return GPIO_PINx 1UL << pin number
  */
__STATIC_INLINE uint32_t GPIO_PINx(PinName pin)
{
    return 1UL << pinNum(pin);
}

/**
  * @brief Configure port pin
  * 
  * @param pin Pin name of the GPIO pin
  * @param conf GPIO_CONF
  * @param mode GPIO_MODE
  * @return true  - success
  * @return false - error
  */
// __STATIC_INLINE bool pinConfigure(PinName pin, GPIO_CONF conf, GPIO_MODE mode)
// {
//     return GPIO_PinConfigure(GPIOx(pin), pinNum(pin), conf, mode);
// }

/**
  * @brief Write a GPIO pin by PinName
  * 
  * @param pin PinName
  * @param val Value (0 or 1)
  */
__STATIC_INLINE void pinWrite(PinName pin, uint32_t val)
{
    if (val)
    {
        LL_GPIO_SetOutputPin(GPIOx(pin), GPIO_PINx(pin));
    }
    else
    {
        LL_GPIO_ResetOutputPin(GPIOx(pin), GPIO_PINx(pin));
    }
}


/**
  * @brief Read a GPIO pin by PinName
  * 
  * @param pin PinName
  * @return pin value (0 or 1) 
  */
__STATIC_INLINE uint32_t pinRead(PinName pin)
{
  return LL_GPIO_IsInputPinSet(GPIOx(pin), GPIO_PINx(pin)) ? 1U : 0U;
}


/**
  * @brief Toggle a GPIO pin by PinName
  * 
  * @param pin PinName
  */
__STATIC_INLINE void pinToggle(PinName pin)
{
    GPIOx(pin)->ODR ^= 1UL << pinNum(pin);
}

__STATIC_INLINE void pinSet(PinName pin)
{
    pinWrite(pin, 1);
}

__STATIC_INLINE void pinReset(PinName pin)
{
    pinWrite(pin, 0);
}

int bsp_gpio_init(void);

void bsp_tacho_exti_enable(void);
void bsp_tacho_exti_disable(void);

void bsp_encoder_exti_enable(void);
void bsp_encoder_exti_disable(void);

void TACHO_IN_TriggerCallback(uint32_t capture_count);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // __BSP_GPIO_H__

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
