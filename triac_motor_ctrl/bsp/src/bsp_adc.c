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

#define ADC_FULLSCALE_VALUE         (__LL_ADC_DIGITAL_SCALE(ADC_RESOLUTION))
#define ADC_RESOLUTION_BITS         (12u - ((ADC_RESOLUTION) << 1u))

#define ADC_CALIBRATION_TIMEOUT_MS  10
#define ADC_CALIBRATION_TIMES       5
#define ADC_REF_CALIBRATION_TIMES   16

#define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(ADC_RESOLUTION) + 1)
/*
 * Public variables
 */

/*
 * Private variables
 */
static __IO uint16_t __raw_datas[ADC_OVERSAMPLING_RATIO] = {0}; // Buffer to store ADC converted data
static __IO _iq16    __filtered_data = 0; // Filtered ADC data after oversampling and filtering
static __IO uint16_t __vref_mv = ADC_REF_VOLTAGE_MV; // Reference voltage in millivolts

/*
 * Private functions
 */
static void __adc_config(void);
static void __adc_config_VREFINT(void);
static void __adc_gpio_config(void);
static void __adc_dma_config(void);
static void __adc_calibration(void);
static void __adc_vref_measurement(void);
static void __quick_sort(int32_t arr[], int32_t length);


__STATIC_INLINE void __adc_enable(void)
{
    /* Enable ADC */
    LL_ADC_Enable(ADC_INSTANCE);

    /* Wait for ADC ready */
    bsp_delay_ms(1);
}


__STATIC_INLINE void __adc_disable(void)
{
    /* Disable ADC */
    LL_ADC_Disable(ADC_INSTANCE);

    /* Wait for ADC disabled */
    bsp_delay_ms(1);
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
    register __IO uint16_t *raw_data = __raw_datas;

    while (len--)
    {
        sum += *raw_data++;
    }

    sum *= (1ul << 16) / ADC_OVERSAMPLING_RATIO;    // Convert to _iq16 format
    return (_iq16)sum;
}


int bsp_adc_init(void)
{
    /* Reset ADC */
    LL_ADC_Reset(ADC_INSTANCE);

    /* ADC clock enable */
    LL_APB1_GRP2_EnableClock(ADC_CLOCK);

    /* ADC calibration */
    __adc_calibration();

    /* ADC external Vref (VCC) measurement */
    __adc_vref_measurement();

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
    LL_ADC_InitTypeDef ADC_InitStruct = { 0 };
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = { 0 };

    /* Configure internal conversion channel */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);

    /*Initialize ADC partial features*/
    ADC_InitStruct.Clock         = ADC_CLOCK_SOURCE;
    ADC_InitStruct.DataAlignment = ADC_DATA_ALIGN;
    ADC_InitStruct.LowPowerMode  = LL_ADC_LP_MODE_NONE;
    ADC_InitStruct.Resolution    = ADC_RESOLUTION;
    LL_ADC_Init(ADC1, &ADC_InitStruct);

    /* Set channel conversion time */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, ADC_SAMPLING_TIME);

    /* Initialize ADC regular features */
    ADC_REG_InitStruct.ContinuousMode   = ADC_CONVERSION_MODE;
    ADC_REG_InitStruct.DMATransfer      = ADC_DMA_TRANSFER_MODE;
    ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.TriggerSource    = ADC_TRIGGER_SOURCE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

    LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);

    /* Set trigger edge as rising edge */
    LL_ADC_REG_SetTriggerEdge(ADC1, ADC_TRIGGER_POLARITY);

    /* Set conversion channel */
    LL_ADC_REG_SetSequencerChannels(ADC1, ADC_CHANNEL);
}


void __adc_config_VREFINT(void)
{
    __IO uint32_t wait_loop_index = 0;
    LL_ADC_InitTypeDef ADC_InitStruct = { 0 };
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = { 0 };

    /*ADC channel and clock source should be configured when ADEN=0, others should be configured when ADSTART=0*/
    /*Initialize ADC partial features*/
    ADC_InitStruct.Clock         = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.LowPowerMode  = LL_ADC_LP_MODE_NONE;
    ADC_InitStruct.Resolution    = LL_ADC_RESOLUTION_12B;
    LL_ADC_Init(ADC1, &ADC_InitStruct);
    
    /* Set channel conversion time */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5);

    /* Initialize ADC regular features */
    ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_CONTINUOUS;
    ADC_REG_InitStruct.DMATransfer      = LL_ADC_REG_DMA_TRANSFER_NONE;
    ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

    LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_BACKWARD);

    /* Set common parameters for ADC */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
    
    /* Wait for stabilization of Vrefint */
    wait_loop_index = ((LL_ADC_DELAY_VREFINT_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while (wait_loop_index != 0)
    {
        wait_loop_index--;
    }

    /* Set channel 12 as conversion channel */
    LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_VREFINT);
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
    LL_DMA_InitTypeDef DMA_InitStruct = { 0 };
    
    /* Enable DMA1 clock */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* Enable syscfg clock */
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

    /* ADC corresponds to channel LL_DMA_CHANNEL_1 */
    LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_ADC);

    DMA_InitStruct.PeriphOrM2MSrcAddress  = (uint32_t)&ADC1->DR;
    DMA_InitStruct.MemoryOrM2MDstAddress  = (uint32_t)&__raw_datas;
    DMA_InitStruct.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    DMA_InitStruct.Mode                   = ADC_DMA_MODE;
    DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
    DMA_InitStruct.MemoryOrM2MDstIncMode  = ADC_DMA_MEMORY_INC_MODE;
    DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
    DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
    DMA_InitStruct.NbData                 = ADC_OVERSAMPLING_RATIO;
    DMA_InitStruct.Priority               = LL_DMA_PRIORITY_HIGH;
    LL_DMA_Init(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL, &DMA_InitStruct);

    /* Enable DMA transfer complete interrupt */
    LL_DMA_EnableIT_TC(ADC_DMA_INSTANCE, ADC_DMA_CHANNEL);

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

void __adc_vref_measurement(void)
{
    uint32_t conv_data = 0;

    __adc_config_VREFINT();

    __adc_enable();

    for (int i = 0; i < ADC_REF_CALIBRATION_TIMES; i++)
    {
        LL_ADC_REG_StartConversion(ADC1);
        while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0);
        conv_data += LL_ADC_REG_ReadConversionData12(ADC1);
        LL_ADC_ClearFlag_EOC(ADC1);
        bsp_delay_ms(10);
    }

    conv_data /= ADC_REF_CALIBRATION_TIMES;

    __vref_mv = __LL_ADC_CALC_VREFANALOG_VOLTAGE((uint16_t)conv_data, LL_ADC_RESOLUTION_12B);

    __adc_disable();

    /* Reset ADC configuration */
    LL_ADC_DeInit(ADC1);
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

        ADC_CompleteCallback(__filtered_data, ADC_RESOLUTION_BITS, __vref_mv);
    }
}


__WEAK void ADC_CompleteCallback(_iq16    filtered_data, 
                                 uint16_t adc_bits, 
                                 uint16_t adc_ref_voltage_mv)
{
    // This function can be overridden by the user to handle ADC conversion complete event
}
/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
