/**
  * @file bsp.h
  * @brief Board support package for MM2000 HW Rev. A
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

#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "RTE_Components.h"
#include CMSIS_device_header
#include "common.h"

#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_buzzer.h"

#ifdef __cplusplus
extern "C" {
#endif /* extern "C" */

#define AC_LINE_FREQ_50_HZ          (50u)
#define AC_LINE_FREQ_60_HZ          (60u)

#define AC_LINE_FREQ                AC_LINE_FREQ_60_HZ
#define SYSTEM_CORE_CLOCK           24000000UL           // 24MHz
#define EXTI_IRQ_PRIORITY           3

// GPIO for LEDs
#define LED_RED                     PB6
#define LED_CPU_STATE               LED_RED

// GPIO for output
#define FAN_CONTROL                 PA2
#define MOTOR_DIR_CONTORL           PA5
#define SCR_GATE_DRIVE              PA4

#define DQ_FAN_CONTROL              0
#define DQ_MOTOR_DIR_CONTROL        1

// GPIO for input
#define LIMIT_SW_UP                 PA6
#define LIMIT_SW_DOWN               PA7
#define LIMIT_SW_STATUS             PA12                // 
#define FOOT_SW_DOWN                PA13
#define FOOT_SW_STATUS              PA14
#define LINE_VOLT_ZCD               PA2
#define ENCODER_A                   PB5
#define ENCODER_B                   PF2

#define ENCODER_A_EXTI_CFG_PORT     LL_EXTI_CONFIG_PORTB
#define ENCODER_A_EXTI_CFG_LINE     LL_EXTI_CONFIG_LINE5
#define ENCODER_A_EXTI_LINE         LL_EXTI_LINE_5
#define ENCODER_A_EXTI_IRQn         EXTI4_15_IRQn

#define ENCODER_B_EXTI_CFG_PORT     LL_EXTI_CONFIG_PORTF
#define ENCODER_B_EXTI_CFG_LINE     LL_EXTI_CONFIG_LINE2
#define ENCODER_B_EXTI_LINE         LL_EXTI_LINE_2
#define ENCODER_B_EXTI_IRQn         EXTI2_3_IRQn  

#define ENCODER_IRQ_PRIORITY        EXTI_IRQ_PRIORITY

// #define LINE_VOLT_ZCD_EXTI          
// #define LINE_VOLT_ZCD_EXTI_LINE     LL_EXTI_LINE_2
// #define LINE_VOLT_ZCD_EXTI_CFG_PORT LL_EXTI_CONFIG_PORTA
// #define LINE_VOLT_ZCD_EXTI_CFG_LINE LL_EXTI_CONFIG_LINE2
// #define LINE_VOLT_ZCD_EXTI_IRQn     EXTI2_3_IRQn

#define DI_LIMIT_SW_UP              0
#define DI_LIMIT_SW_DOWN            1
#define DI_LIMIT_SW_STATUS          2
#define DI_FOOT_SW_DOWN             3
#define DI_FOOT_SW_STATUS           4

#define STATE_SWITCH_ON             0
#define STATE_SWITCH_OFF            1

#define IS_SWITCH_ON(state)         ((state) == STATE_SWITCH_ON)
#define IS_SWITCH_OFF(state)        ((state) == STATE_SWITCH_OFF)

// GPIO for motor speed measurement
#define TACHO_INPUT                 PB7
#define TACHO_INPUT_EXTI_CFG_PORT   LL_EXTI_CONFIG_PORTB
#define TACHO_INPUT_EXTI_CFG_LINE   LL_EXTI_CONFIG_LINE7
#define TACHO_INPUT_EXTI_LINE       LL_EXTI_LINE_7
#define TACHO_INPUT_EXTI_IRQn       EXTI4_15_IRQn
#define TACHO_INPUT_IRQ_PRIORITY    EXTI_IRQ_PRIORITY

// GPIO for analog input
#define ADC1_IN0                    PA0     // for motor current measurement and limit

// GPIO for timer
#define TIM1_CH4                    PA1     // for buzzer, 4kHz 50% duty cycle
#define TIM3_CH1                    PA2     // for line voltage zero cross detection and frequency measurement
#define TIM3_CH3                    PA4     // for motor speed control
#define TIM17_CH1N                  PB7     // for motor speed measurement

// GPIO for I2C
#define I2C_SCL                     PF1
#define I2C_SDA                     PF0   

// GPIO for MCO
#define MCO_PIN                     PA1

__STATIC_INLINE void led_on(PinName led)
{
    pinSet(led);
}

__STATIC_INLINE void led_off(PinName led)
{
    pinReset(led);
}

__STATIC_INLINE void led_toggle(PinName led)
{
    pinToggle(led);
}


/*
 * I2C 
 */
#define I2C_FREQUENCY               100000
#define I2C_DUTY_CYCLE              I2C_DUTYCYCLE_16_9
#define I2C_HANDLER                 &hi2c1

/*
 * USB
 */

/*
 * Use MCO for buzzer clock source
 *     MOC output = LSI / 8 = 4096Hz
 */ 
#define BUZZER_CLK_SOURCE           LL_RCC_MCO1SOURCE_LSI
#define BUZZER_CLK_DIV              LL_RCC_MCO1_DIV_8
#define BUZZER_CLK_PIN              MCO_PIN
#define BUZZER_CLK_PIN_AF           LL_GPIO_AF15_MCO

/*
 * ADC
 */
#define ADC_CLOCK                   LL_APB2_GRP1_PERIPH_ADC
#define ADC_INSTANCE                ADC1
#define ADC_CLOCK_SOURCE            LL_ADC_CLOCK_SYNC_PCLK_DIV8     // 3MHz
#define ADC_CHANNEL                 LL_ADC_CHANNEL_0
#define ADC_RESOLUTION              LL_ADC_RESOLUTION_12B           // 12-bit
#define ADC_SAMPLING_TIME           LL_ADC_SAMPLINGTIME_13CYCLES_5  // 13.5 ADC clock cycles
#define ADC_TRIGGER_SOURCE          LL_ADC_REG_TRIG_EXT_TIM1_CH4
#define ADC_TRIGGER_POLARITY        LL_ADC_REG_TRIG_EXT_RISING
#define ADC_CONVERSION_MODE         LL_ADC_CONV_SINGLE
#define ADC_OVERSAMPLING_RATIO      (64u)
#define ADC_CHANNEL_GPIO_PIN        ADC1_IN0

/*
 * TIM1 for ADC oversampling trigger
 */
#define ADC_TIM_CLOCK               LL_APB1_GRP2_PERIPH_TIM1
#define ADC_TIM_INSTANCE            TIM1
#define ADC_TIM_CHANNEL             LL_TIM_CHANNEL_CH4
#define ADC_TIM_FREQUENCY           AC_LINE_FREQ * 2 * ADC_OVERSAMPLING_RATIO   
#define ADC_TIM_AUTORELOAD          (SYSTEM_CORE_CLOCK) / (ADC_TIM_FREQUENCY) - 1UL
#define ADC_TIM_TRGO_SOURCE         LL_TIM_TRGO_OC4REF
#define ADC_TIM_TRIGGER_IN          LL_TIM_TS_ITR2                  // TIM3 TRGO -> TIM1 ITR2

/*
 * TIM3 for line voltage zero cross detection and SCR gate drive timing
 */
#define ZCD_TIM_CLOCK               LL_APB1_GRP1_PERIPH_TIM3
#define ZCD_TIM_INSTANCE            TIM3
#define ZCD_TIM_CHANNEL             LL_TIM_CHANNEL_CH1
// #define ZCD_TIM_IC_POLARITY         LL_TIM_IC_POLARITY_RISING
#define ZCD_TIM_IC_POLARITY         LL_TIM_IC_POLARITY_FALLING
// #define ZCD_TIM_IC_POLARITY         LL_TIM_IC_POLARITY_BOTHEDGE
#define ZCD_TIM_IC_FILTER           LL_TIM_IC_FILTER_FDIV1_N2
#define ZCD_TIM_PIN                 TIM3_CH1
#define ZCD_TIM_PIN_AF              LL_GPIO_AF13_TIM3
#define ZCD_TIM_FREQUENCY           600000UL            // 600KHz
#define ZCD_TIM_PRESCALER           (SYSTEM_CORE_CLOCK / ZCD_TIM_FREQUENCY - 1UL)
#define ZCD_TIM_AUTORELOAD          (ZCD_TIM_FREQUENCY / (2 * AC_LINE_FREQ) - 1UL)
#define ZCD_TIM_AUTORELOAD_COMPENSATION 125u
#define ZCD_TIM_TRGO_SOURCE         LL_TIM_TRGO_ENABLE
#define ZCD_TIM_IRQn                TIM3_IRQn
#define ZCD_TIM_IRQHandler          TIM3_IRQHandler
#define ZCD_TIM_IRQ_PRIORITY        0

#define GATE_DRV_TIM_CHANNEL        LL_TIM_CHANNEL_CH3
#define GATE_DRV_TIM_PWM_MODE       LL_TIM_OCMODE_PWM2
#define GATE_DRV_TIM_PIN            TIM3_CH3
#define GATE_DRV_TIM_PIN_AF         LL_GPIO_AF13_TIM3

/*
 * TIM17 for motor speed measurement
 */
#define TACHO_TIM_CLOCK             LL_APB1_GRP2_PERIPH_TIM17
#define TACHO_TIM_INSTANCE          TIM17
#define TACHO_TIM_CHANNEL           LL_TIM_CHANNEL_CH1N
#define TACHO_TIM_IC_POLARITY       LL_TIM_IC_POLARITY_FALLING
#define TACHO_TIM_PIN               TIM17_CH1N
#define TACHO_TIM_PIN_AF            LL_GPIO_AF2_TIM17
#define TACHO_TIM_FREQUENCY         2000000UL           // 2MHz, for 0.5us resolution
#define TACHO_TIM_PRESCALER         (SYSTEM_CORE_CLOCK / TACHO_TIM_FREQUENCY - 1UL)
#define TACHO_TIM_AUTORELOAD        0xFFFF
#define TACHO_TIM_IRQn              TIM17_IRQn
#define TACHO_TIM_IRQHandler        TIM17_IRQHandler
#define TACHO_TIM_IRQ_PRIORITY      3

int bsp_init(void);

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif // __BSP_H__

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
