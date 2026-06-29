/**
  * @file bsp_timer.c
  * @brief 
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2026/06/15
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
  * 	2026/06/15	0.1			Leon Lee		Preliminary version.
  */

#include "RTE_Components.h"
#include CMSIS_device_header
#include "bsp.h"
#include "py32f0xx_ll.h"
#include "bsp_timer.h"

#define TIM3_CLK_FREQ           6000000UL       // TIM3 clock frequency in Hz (6MHz)

static int __config_zcd_timer(void);
static int __config_gate_drv_timer(void);
static int __config_adc_timer(void);
static int __config_tacho_timer(void);

int bsp_timer_init(void)
{
    __config_zcd_timer();
    __config_adc_timer();
    __config_tacho_timer();
    
    return SUCCESS;
}


void bsp_zcd_timer_start(void)
{
    CRITICAL_SECTION_BEGIN();

    /* Clear ZCD timer counter*/
    LL_TIM_GenerateEvent_UPDATE(ZCD_TIM_INSTANCE);

    /* Clear ZCD timer all interrupt flags */
    LL_TIM_ClearFlag_UPDATE(ZCD_TIM_INSTANCE);
    LL_TIM_ClearFlag_CC1(ZCD_TIM_INSTANCE);
    LL_TIM_ClearFlag_CC3(ZCD_TIM_INSTANCE);

    /* Enable ZCD timer CC1 interrupt */
    // LL_TIM_EnableIT_CC1(ZCD_TIM_INSTANCE);
    LL_TIM_EnableIT_TRIG(ZCD_TIM_INSTANCE);

    /* ZCD timer NVIC configuration */
    NVIC_ClearPendingIRQ(ZCD_TIM_IRQn);
    NVIC_SetPriority(ZCD_TIM_IRQn, ZCD_TIM_IRQ_PRIORITY);
    NVIC_EnableIRQ(ZCD_TIM_IRQn);

    /* Enable ZCD timer channel */
    LL_TIM_CC_EnableChannel(ZCD_TIM_INSTANCE, ZCD_TIM_CHANNEL);
    /* Enable gate driver timer channel  */
    LL_TIM_CC_EnableChannel(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL);
    /* Enable ZCD timer counter */
    LL_TIM_EnableCounter(ZCD_TIM_INSTANCE);

    CRITICAL_SECTION_END();
}


void bsp_zcd_timer_stop(void)
{
    CRITICAL_SECTION_BEGIN();

    /* Disable ZCD timer counter */
    LL_TIM_DisableCounter(ZCD_TIM_INSTANCE);
    /* Disable ZCD timer CC1 interrupt */
    // LL_TIM_DisableIT_CC1(ZCD_TIM_INSTANCE);
    LL_TIM_DisableIT_TRIG(ZCD_TIM_INSTANCE);

    /* Disable ZCD timer interrupt */
    NVIC_DisableIRQ(ZCD_TIM_IRQn);

    CRITICAL_SECTION_END();
}


void bsp_adc_timer_start(void)
{
    /* Reset ADC timer counter */
    LL_TIM_GenerateEvent_UPDATE(ADC_TIM_INSTANCE);
    
    CRITICAL_SECTION_BEGIN();

    /* Clear ADC timer all interrupt flags */
    LL_TIM_ClearFlag_UPDATE(ADC_TIM_INSTANCE);
    
    /* Enable ADC timer interrupt */
    LL_TIM_EnableIT_UPDATE(ADC_TIM_INSTANCE);
    
    /* ADC timer NVIC configuration */
    NVIC_ClearPendingIRQ(ADC_TIM_IRQn);
    NVIC_SetPriority(ADC_TIM_IRQn, ADC_TIM_IRQ_PRIORITY);
    NVIC_EnableIRQ(ADC_TIM_IRQn);

    CRITICAL_SECTION_END();
    
    /* Enable ADC timer channel */
    LL_TIM_CC_EnableChannel(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL);
    
    /* Enable ADC timer counter */
    LL_TIM_EnableCounter(ADC_TIM_INSTANCE);
}


void bsp_adc_timer_stop(void)
{
    /* Disable ADC timer channel */
    LL_TIM_CC_DisableChannel(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL);

    /* Disable ADC timer counter */
    LL_TIM_DisableCounter(ADC_TIM_INSTANCE);
}

void bsp_tacho_timer_start(void)
{
    
    /* Reset TACHO timer counter */
    LL_TIM_GenerateEvent_UPDATE(TACHO_TIM_INSTANCE);
    
    /* Clear TACHO timer all interrupt flags */
    LL_TIM_ClearFlag_UPDATE(TACHO_TIM_INSTANCE);
    
    /* Enable TACHO timer interrupt */
    LL_TIM_EnableIT_UPDATE(TACHO_TIM_INSTANCE);
    
    CRITICAL_SECTION_BEGIN();

    /* TACHO timer NVIC configuration */
    NVIC_ClearPendingIRQ(TACHO_TIM_IRQn);
    NVIC_SetPriority(TACHO_TIM_IRQn, TACHO_TIM_IRQ_PRIORITY);
    NVIC_EnableIRQ(TACHO_TIM_IRQn);

    /* Enable TACHO timer counter */
    LL_TIM_EnableCounter(TACHO_TIM_INSTANCE);

    CRITICAL_SECTION_END();
}


void bsp_tacho_timer_stop(void)
{
    CRITICAL_SECTION_BEGIN();

    /* Disable TACHO timer counter */
    LL_TIM_DisableCounter(TACHO_TIM_INSTANCE);
    
    /* Disable TACHO timer interrupt */
    NVIC_DisableIRQ(TACHO_TIM_IRQn);

    CRITICAL_SECTION_END();
}


__WEAK void ZCD_TIM_TRIG_Callback(void)
{
}


__WEAK void ZCD_TIM_CC1_Callback(void)
{
}


__WEAK void TACHO_TIM_UpdateCallback(void)
{
}


static int __config_zcd_timer(void)
{
    // Enable ZCD timer clock
    LL_APB1_GRP1_EnableClock(ZCD_TIM_CLOCK);

    // Enable ZCD and gate driver ports clock
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    // Get ZCD timer clock frequency
    SystemCoreClockUpdate();
    
    // Ensure SystemCoreClock is updated and equal to 24MHz
    assert(SystemCoreClock == 24000000UL && "System clock is not equal to 24MHz"); 

    // Configure ZCD timer as one shot mode
    LL_TIM_DisableCounter(ZCD_TIM_INSTANCE);

    /* CK_INT not divided */
    LL_TIM_SetClockDivision(ZCD_TIM_INSTANCE, LL_TIM_CLOCKDIVISION_DIV1);

    /* Up counting */
    LL_TIM_SetCounterMode(ZCD_TIM_INSTANCE, LL_TIM_COUNTERMODE_UP);

    /* Set auto-reload value to ZCD_TIM_AUTORELOAD */
    LL_TIM_SetAutoReload(ZCD_TIM_INSTANCE, ZCD_TIM_AUTORELOAD - ZCD_TIM_AUTORELOAD_COMPENSATION);

    /* CK_CNT Prescaler value */
    LL_TIM_SetPrescaler(ZCD_TIM_INSTANCE, ZCD_TIM_PRESCALER);

    /* Set ZCD timer as slave mode trigger */
    LL_TIM_SetSlaveMode(ZCD_TIM_INSTANCE,LL_TIM_SLAVEMODE_TRIGGER);
  
    /* Set input trigger source */
    LL_TIM_SetTriggerInput(ZCD_TIM_INSTANCE, ZCD_TIM_TRIGGER_IN);

    /* Trigger/output channel configuration */
    __config_gate_drv_timer();

    /* Set one-pulse mode */
    LL_TIM_SetOnePulseMode(ZCD_TIM_INSTANCE,LL_TIM_ONEPULSEMODE_SINGLE);
  
    /* Disable automatic output */
    LL_TIM_DisableAutomaticOutput(ZCD_TIM_INSTANCE);

    /* Enable main output */
    LL_TIM_EnableAllOutputs(ZCD_TIM_INSTANCE);
    
    /* Set TI1FP1 as TRGO source */
    LL_TIM_SetTriggerOutput(ZCD_TIM_INSTANCE, LL_TIM_TRGO_ENABLE);
    /* Enable master/slave mode */
    LL_TIM_EnableMasterSlaveMode(ZCD_TIM_INSTANCE);

    /* Enable ZCD timer counter */
    // LL_TIM_EnableCounter(ZCD_TIM_INSTANCE);

    return SUCCESS;
}

static int __config_gate_drv_timer(void)
{
    /* ZCD timer channel configuration */
    /* Configure active edge as rising edge */
    LL_TIM_IC_SetPolarity(ZCD_TIM_INSTANCE, ZCD_TIM_CHANNEL, ZCD_TIM_IC_POLARITY);

    /* Configure channel as input mode */
    LL_TIM_IC_SetActiveInput(ZCD_TIM_INSTANCE, ZCD_TIM_CHANNEL, LL_TIM_ACTIVEINPUT_DIRECTTI);
    /* Configure channel input filter */
    LL_TIM_IC_SetFilter(ZCD_TIM_INSTANCE, ZCD_TIM_CHANNEL, ZCD_TIM_IC_FILTER);
    
    /* ZCD timer channel input mapped to PA2 */
    // LL_GPIO_SetPinMode(GPIOx(ZCD_TIM_PIN), GPIO_PINx(ZCD_TIM_PIN), LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(GPIOx(ZCD_TIM_PIN), GPIO_PINx(ZCD_TIM_PIN), LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOx(ZCD_TIM_PIN), GPIO_PINx(ZCD_TIM_PIN), LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOx(ZCD_TIM_PIN), GPIO_PINx(ZCD_TIM_PIN), ZCD_TIM_PIN_AF);

    /* Enable ZCD timer channel */
    // LL_TIM_CC_EnableChannel(ZCD_TIM_INSTANCE, ZCD_TIM_CHANNEL);

    /* Gate driver timer channel configuration */
    /* Set output polarity to high */
    LL_TIM_OC_SetPolarity(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL, LL_TIM_OCPOLARITY_HIGH);
    /* Set idle state to low */
    LL_TIM_OC_SetIdleState(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL, LL_TIM_OCIDLESTATE_LOW);
    /* Enable compare preload */
    LL_TIM_OC_EnablePreload(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL);
    /* Set compare value to MAX */
    // LL_TIM_OC_SetCompareCH3(ZCD_TIM_INSTANCE, ZCD_TIM_AUTORELOAD / 4);
    LL_TIM_OC_SetCompareCH3(ZCD_TIM_INSTANCE, 1);
    /* Configure channel as PWM2 mode */
    LL_TIM_OC_SetMode(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL, LL_TIM_OCMODE_PWM2);

    /* Output mapped to gate drive pin */
    LL_GPIO_SetPinMode(GPIOx(GATE_DRV_TIM_PIN), GPIO_PINx(GATE_DRV_TIM_PIN), LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOx(GATE_DRV_TIM_PIN), GPIO_PINx(GATE_DRV_TIM_PIN), GATE_DRV_TIM_PIN_AF);


    /* Enable gate driver timer channel  */
    // LL_TIM_CC_EnableChannel(ZCD_TIM_INSTANCE, GATE_DRV_TIM_CHANNEL);

    return SUCCESS;
}

int __config_adc_timer(void)
{
    /* ADC timer configuration */
    /* Enable Timer clock */
    LL_APB1_GRP2_EnableClock(ADC_TIM_CLOCK);

    /* Disable Timer */
    LL_TIM_DisableCounter(ADC_TIM_INSTANCE);

    /* CK_INT not divided */
    LL_TIM_SetClockDivision(ADC_TIM_INSTANCE, LL_TIM_CLOCKDIVISION_DIV1);

    /* Up counting */
    LL_TIM_SetCounterMode(ADC_TIM_INSTANCE, LL_TIM_COUNTERMODE_UP);

    /* Set auto-reload value to ZCD_TIM_AUTORELOAD */
    LL_TIM_SetAutoReload(ADC_TIM_INSTANCE, ADC_TIM_AUTORELOAD);

    /* CK_CNT Prescaler value */
    LL_TIM_SetPrescaler(ADC_TIM_INSTANCE, 0UL);

    /* Set ADC timer as slave mode trigger */
    LL_TIM_SetSlaveMode(ADC_TIM_INSTANCE, LL_TIM_SLAVEMODE_RESET);
  
    /* Set input trigger source as ITR2 */
    LL_TIM_SetTriggerInput(ADC_TIM_INSTANCE, ADC_TIM_TRIGGER_IN);

    /* TRGO output channel configuration */
    /* Set output polarity to high */
    LL_TIM_OC_SetPolarity(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL, LL_TIM_OCPOLARITY_HIGH);
    /* Set idle state to low */
    LL_TIM_OC_SetIdleState(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL, LL_TIM_OCIDLESTATE_LOW);
    /* Enable compare preload */
    LL_TIM_OC_EnablePreload(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL);
    /* Set compare value to 50% of auto-reload */
    LL_TIM_OC_SetCompareCH4(ADC_TIM_INSTANCE, ADC_TIM_AUTORELOAD / 2);
    /* Configure channel as PWM2 mode */
    LL_TIM_OC_SetMode(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL, LL_TIM_OCMODE_PWM1);
    /* Set trigger output */
    LL_TIM_SetTriggerOutput(ADC_TIM_INSTANCE, ADC_TIM_TRGO_SOURCE);

    /* Enable TRGO output channel */
    LL_TIM_CC_EnableChannel(ADC_TIM_INSTANCE, ADC_TIM_CHANNEL);

    /* Enable master/slave mode */
    LL_TIM_EnableMasterSlaveMode(ADC_TIM_INSTANCE);

    return 0;
}

int __config_tacho_timer(void)
{
    /* TACHO timer configuration */
    /* Enable Timer clock */
    LL_APB1_GRP2_EnableClock(TACHO_TIM_CLOCK);

    /* Disable Timer */
    LL_TIM_DisableCounter(TACHO_TIM_INSTANCE);

    /* CK_INT not divided */
    LL_TIM_SetClockDivision(TACHO_TIM_INSTANCE, LL_TIM_CLOCKDIVISION_DIV1);

    /* Up counting */
    LL_TIM_SetCounterMode(TACHO_TIM_INSTANCE, LL_TIM_COUNTERMODE_UP);

    /* Set auto-reload value to TACHO_TIM_AUTORELOAD */
    LL_TIM_SetAutoReload(TACHO_TIM_INSTANCE, TACHO_TIM_AUTORELOAD);

    /* CK_CNT Prescaler value */
    LL_TIM_SetPrescaler(TACHO_TIM_INSTANCE, TACHO_TIM_PRESCALER);

    return 0;
}

void ZCD_TIM_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_TRIG(ZCD_TIM_INSTANCE) != 0U &&
        LL_TIM_IsEnabledIT_TRIG(ZCD_TIM_INSTANCE) != 0U)
    {
        LL_TIM_ClearFlag_TRIG(ZCD_TIM_INSTANCE);

        // Handle zero cross detection event here
        ZCD_TIM_TRIG_Callback();
    }

    if (LL_TIM_IsActiveFlag_CC1(ZCD_TIM_INSTANCE) != 0U &&
        LL_TIM_IsEnabledIT_CC1(ZCD_TIM_INSTANCE) != 0U)
    {
        LL_TIM_ClearFlag_CC1(ZCD_TIM_INSTANCE);

        // Handle zero cross detection event here
        ZCD_TIM_CC1_Callback();
    }
}

void TACHO_TIM_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TACHO_TIM_INSTANCE) != 0U &&
        LL_TIM_IsEnabledIT_UPDATE(TACHO_TIM_INSTANCE) != 0U)
    {
        LL_TIM_ClearFlag_UPDATE(TACHO_TIM_INSTANCE);

        // Handle tacho event here
        TACHO_TIM_UpdateCallback();
    }
}

void ADC_TIM_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(ADC_TIM_INSTANCE) != 0U &&
        LL_TIM_IsEnabledIT_UPDATE(ADC_TIM_INSTANCE) != 0U)
    {
        LL_TIM_ClearFlag_UPDATE(ADC_TIM_INSTANCE);

        // Handle ADC timer event here
        // User-defined callback function can be called here
        pinToggle(LED_CPU_STATE);
    }
}


void TACHO_hwStart(void)
{
    bsp_tacho_timer_start();
    bsp_tacho_exti_enable();
}


void TACHO_hwStop(void)
{
    bsp_tacho_exti_disable();
    bsp_tacho_timer_stop();
}

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
