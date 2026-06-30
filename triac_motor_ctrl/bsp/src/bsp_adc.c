/**
  * @file bsp_adc.c
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

#include "py32f0xx_ll.h" 
#include "bsp.h"
#include "bsp_adc.h"


#define ADC_CALIBRATION_TIMEOUT_MS  10
#define ADC_CALIBRATION_TIMES       5

/*
 * Public variables
 */

/*
 * Private variables
 */
static __IO uint16_t __raw_datas[ADC_OVERSAMPLING_RATIO] = {0}; // Buffer to store ADC converted data
static __IO _iq16    __filtered_data = 0; // Filtered ADC data after oversampling and filtering

static void __adc_config(void);
static void __adc_gpio_config(void);
static void __adc_dma_config(void);
static void __adc_calibration(void);
static void __quick_sort(int32_t arr[], int32_t length);

__STATIC_INLINE void __adc_stop(void);
__STATIC_INLINE _iq16 __filter(void);

int bsp_adc_init(void)
{
    /* Reset ADC */
    LL_ADC_Reset(ADC_INSTANCE);

    /* ADC clock enable */
    LL_APB1_GRP2_EnableClock(ADC_CLOCK);

    /* ADC calibration */
    __adc_calibration();

    /* ADC GPIO config */
    __adc_gpio_config();

    /* ADC config */
    __adc_config();

    /* ADC DMA config */
    __adc_dma_config();

    return SUCCESS;
}


void bsp_adc_start(void)
{
    /* Disable DMA transfer */
    LL_DMA_DisableChannel(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL);
    /* Reset DMA transfer data counter */
    LL_DMA_SetDataLength(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL, ADC_OVERSAMPLING_RATIO);
    /* Enable DMA transfer */
    LL_DMA_EnableChannel(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL);

    /* Enable ADC */
    LL_ADC_Enable(ADC_INSTANCE);
    /* Start ADC conversion */
    LL_ADC_REG_StartConversion(ADC_INSTANCE);
}

void bsp_adc_stop(void)
{
    __adc_stop();
}

void bsp_adc_restart(void)
{
    /* Reset DMA transfer data counter */
    LL_DMA_SetDataLength(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL, ADC_OVERSAMPLING_RATIO);
    /* Enable DMA transfer */
    LL_DMA_EnableChannel(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL);

    /* Start ADC conversion */
    LL_ADC_REG_StartConversion(ADC_INSTANCE);
}


static void __adc_config(void)
{
    /* ADC channel and clock source should be configured when ADEN=0, others should be configured when ADSTART=0 */
    /* Configure internal conversion channel */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);

    /* Set ADC clock */
    LL_ADC_SetClock(ADC1, ADC_CLOCK_SOURCE);

    /* Set 12-bit resolution */
    LL_ADC_SetResolution(ADC1, ADC_RESOLUTION);

    /* Right-alignment for converted data */
    LL_ADC_SetDataAlignment(ADC1, ADC_DATA_ALIGN);

    /* Set low power mode to none */
    LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);

    /* Set channel conversion time */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, ADC_SAMPLING_TIME);

    /* Set the trigger source as TIM1 CH4 */
    LL_ADC_REG_SetTriggerSource(ADC1, ADC_TRIGGER_SOURCE);

    /* Set trigger edge as rising edge */
    LL_ADC_REG_SetTriggerEdge(ADC1, ADC_TRIGGER_POLARITY);

    /* Single sampling */
    LL_ADC_REG_SetContinuousMode(ADC1, ADC_CONVERSION_MODE);

    /* Set DMA mode to circular mode and enable it */
    LL_ADC_REG_SetDMATransfer(ADC1, ADC_DMA_TRANSFER_MODE);

    /* Set overrun management mode to data overwritten */
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

    /* Set discontinuous mode to disabled */
    LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);

    /* Set conversion channel */
    LL_ADC_REG_SetSequencerChannels(ADC1, ADC_CHANNEL);
}

static void __adc_gpio_config(void)
{
    /* ADC GPIO config */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOx(ADC1_IN0), GPIO_PINx(ADC1_IN0), LL_GPIO_MODE_ANALOG);
}


static void __adc_dma_config(void)
{
    /* ADC DMA config */
    /* Enable DMA1 clock */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* Enable syscfg clock */
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

    /* ADC corresponds to channel LL_DMA_CHANNEL_1 */
    LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_ADC);

    /* Configure DMA data transfer direction as peripheral to memory */
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    /* Configure DMA priority as high */
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);

    /* Configure DMA in non-circular mode */
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, ADC_DMA_MODE);

    /* Configure DMA peripheral increment mode as no increment */
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

    /* Configure DMA memory increment mode as increment */
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, ADC_DMA_MEMORY_INC_MODE);

    /* Configure DMA peripheral data size as half-word */
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);

    /* Configure DMA memory data size as half-word */
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);

    /* Configure DMA transfer length to ADC_OVERSAMPLING_RATIO */
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_OVERSAMPLING_RATIO);

    /* Configure DMA peripheral and memory addresses */
    LL_DMA_ConfigAddresses(DMA1, 
                           LL_DMA_CHANNEL_1, 
                           (uint32_t)&ADC1->DR, 
                           (uint32_t)&__raw_datas, 
                           LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));

    /* Enable DMA transfer complete interrupt */
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

    NVIC_SetPriority(ADC_DMA_IRQn, ADC_DMA_IRQ_PRIORITY);
    NVIC_EnableIRQ(ADC_DMA_IRQn);
}


void __adc_calibration(void)
{
    __IO uint32_t backup_setting_adc_dma_transfer = 0;
    uint32_t ADC_CALRR1_Buff[ADC_CALIBRATION_TIMES];
    int32_t ADC_CALRR1_BuffInt[ADC_CALIBRATION_TIMES];
#if (USE_TIMEOUT == 1)
    uint32_t Timeout = 0;
#endif /* USE_TIMEOUT */

    if (LL_ADC_IsEnabled(ADC1) != 0) return;

    /* Disable ADC DMA transfer during calibration */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);

    for (int i = 0; i < ADC_CALIBRATION_TIMES; i++)
    {
        /* Enable calibration */
        LL_ADC_StartCalibration(ADC1);

#if (USE_TIMEOUT == 1)
        Timeout = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

        while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
        {
#if (USE_TIMEOUT == 1)
            /* Check if calibration is timeout */
            if (LL_SYSTICK_IsActiveCounterFlag())
            {
                if (Timeout-- == 0)
                {
                    return;
                }
            }
#endif /* USE_TIMEOUT */
        }

        ADC_CALRR1_Buff[i] = *((__IO uint32_t*)(0x40012448));
        ADC_CALRR1_BuffInt[i] = (int32_t)(ADC_CALRR1_Buff[i] << 9);
    }

    __quick_sort(ADC_CALRR1_BuffInt, ADC_CALIBRATION_TIMES);
    *((__IO uint32_t*)(0x40012450)) = (ADC_CALRR1_BuffInt[2] >> 9); // middle value of 5 calibration results
    *((__IO uint32_t*)(0x40012454)) = *((__IO uint32_t*)(0x4001244C));
    *((__IO uint32_t*)(0x40012444)) |= 0x8000;
    
    /* Delay between ADC calibration end and ADC enable: minimum 4 ADC Clock cycles */
    bsp_delay_ms(1);

    /* Restore ADC DMA configuration */
    LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);
}

__STATIC_INLINE void __adc_stop(void)
{
    // Stop ADC conversion
    LL_ADC_REG_StopConversion(ADC_INSTANCE);

    // Disable DMA transfer
    LL_DMA_DisableChannel(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL);
}


__STATIC_INLINE _iq16 __filter(void)
{
    register int32_t sum = 0;
    register uint32_t len = ADC_OVERSAMPLING_RATIO;
    register uint16_t *raw_data = __raw_datas;

    while (len--)
    {
        sum += *raw_data++;
    }

    sum *= (1ul << 16) / ADC_OVERSAMPLING_RATIO;    // Convert to _iq16 format
    return (_iq16)sum;
}


static void __quick_sort(int32_t arr[], int32_t length)
{
    int32_t pos, min;

    for (int32_t i = 0;i < length;i++)
    {
        min = arr[i];
        pos = i;

        for (int32_t j = i + 1;j < length;j++)
        {
            if (min > arr[j])
            {
                min = arr[j];
                pos = j;
            }
        }

        arr[pos] = arr[i];
        arr[i] = min;
    }
}


void ADC_DMA_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC1(ADC_DMA_INSTANCE) != 0U && 
        LL_DMA_IsEnabledIT_TC(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL) != 0U)
    {
        LL_DMA_ClearFlag_TC1(ADC_DMA_INSTANCE);

        __adc_stop(); // Stop ADC conversion and DMA transfer

        // Process the ADC data in __raw_datas buffer
        // For example, you can calculate the average or apply filtering
        __filtered_data = __filter();

        ADC_CompleteCallback(__filtered_data);
    }
}

__WEAK void ADC_CompleteCallback(_iq16 filtered_data)
{
    // This function can be overridden by the user to handle ADC conversion complete event
}
/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
