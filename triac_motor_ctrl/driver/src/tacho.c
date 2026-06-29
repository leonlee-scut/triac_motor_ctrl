/**
  * @file tacho.c
  * @brief Source file for tacho driver functions and definitions.
  * @author Leon Lee (leonlee.scut@outlook.com)
  * @version 0.1
  * @date 2026/06/26
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
  * 	2026/06/26	0.1			Leon Lee		Preliminary version.
  */

/**
  * @note In this implementation, tacho speed is calculated based on the time
  *       interval between two consecutive capture events. The speed is calculated
  *       using the formula: 
  *
  *                        60       60 x Fref       CONSTANT_CONVERT_COEFF
  *                RPM = ------ = ------------- = --------------------------
  *                       Trev     PPR x dCNT               dCNT
  *
  *      where:
  *          Trev = time interval between two consecutive capture events
  *          Fref = reference clock frequency
  *          PPR  = pulse per rotation
  *          dCNT = number of counts between two consecutive capture events
  */
 
/**
  * @note The parameter "pulse_per_rev" should not be greater than 128. 
  */ 
 
#include "tacho.h"
#include "common.h"

#define TACHO_MAX_OVF_COUNT  50  // Maximum number of overflows allowed before considering the speed as zero
/*
 * Private variables
 */
volatile uint32_t __ref_freq = 0;       // Reference clock frequency in Hz
volatile uint8_t  __pulse_per_rev = 0;  // Pulse per revolution
volatile uint16_t __last_cap_cnt = 0;   // Last capture counter value
volatile uint8_t  __is_first_cap = 0;   // Flag to indicate if the first capture event has occurred
volatile _iq16    __rpm = 0;            // Tacho speed in RPM (IQ16 format)

volatile uint16_t __ovf_cnt = 0;        // Overflow counter for handling timer overflows
volatile int64_t  __convert_coeff = 0;  // Conversion coefficient for RPM calculation


__STATIC_INLINE int64_t __calc_rpm_coeff(uint32_t ref_clk_freq, uint8_t pluse_per_rev)
{
    volatile register int64_t rpm_coeff = 60 << 16; // Scale to IQ16 format
    rpm_coeff *= ref_clk_freq;
    rpm_coeff /= pluse_per_rev;

    return rpm_coeff;
}

void TACHO_init(uint32_t ref_clk_freq, uint8_t pluse_per_rev)
{
    assert(ref_clk_freq > 0 && "Reference clock frequency must be greater than zero");
    assert(pluse_per_rev > 0 && "Pulse per revolution must be greater than zero");

    __ref_freq = ref_clk_freq;
    __pulse_per_rev = pluse_per_rev;
    __last_cap_cnt = 0;
    __is_first_cap = 1;
    __rpm = 0;
    __ovf_cnt = 0;

    // Pre-calculate the conversion coefficient for RPM calculation
    __convert_coeff = __calc_rpm_coeff(__ref_freq, __pulse_per_rev);

    TACHO_hwInit();
}


void TACHO_start(void)
{
    TACHO_reset();
    TACHO_hwStart();
}


void TACHO_stop(void)
{
    TACHO_hwStop();
}


void TACHO_reset(void)
{
    __last_cap_cnt = 0;
    __is_first_cap = 0;
    __rpm = 0;
    __ovf_cnt = 0;

    TACHO_hwReset();
}


_iq16 TACHO_getSpeedRPM(void)
{
    if (__ovf_cnt > TACHO_MAX_OVF_COUNT)
    {
        return 0; // If there are too many overflows, return 0 RPM
    }

    return __rpm;
}

void TACHO_setRefClkFreq(uint32_t ref_clk_freq)
{
    assert(ref_clk_freq > 0 && "Reference clock frequency must be greater than zero");
    __ref_freq = ref_clk_freq;

    // Pre-calculate the conversion coefficient for RPM calculation
    __convert_coeff = __calc_rpm_coeff(__ref_freq, __pulse_per_rev);
}

void TACHO_setPlusePerRev(uint8_t pluse_per_rev)
{
    assert(pluse_per_rev > 0 && "Pulse per revolution must be greater than zero");
    __pulse_per_rev = pluse_per_rev;

    // Pre-calculate the conversion coefficient for RPM calculation
    __convert_coeff = __calc_rpm_coeff(__ref_freq, __pulse_per_rev);
}


uint32_t TACHO_getRefClkFreq(void)
{
    return __ref_freq;
}

uint8_t TACHO_getPlusePerRev(void)
{
    return __pulse_per_rev;
}


void TACHO_handleCaptureEvent(uint16_t cap_cnt) 
{
    if (__is_first_cap == 0)
    {
        __last_cap_cnt = cap_cnt;
        __ovf_cnt = 0;
        __is_first_cap = 1;
        return;
    }

    // Calculate the difference in counter value between two consecutive capture events
    uint32_t __diff_cnt = (((uint32_t)__ovf_cnt << 16) | cap_cnt ) - __last_cap_cnt; 

    if (__diff_cnt > 0)
    {
        // Calculate RPM using the formula: RPM = (60 * Fref) / (PPR * dCNT)
        __rpm = (_iq16)(__convert_coeff / __diff_cnt);
    }

    __last_cap_cnt = cap_cnt;
    __ovf_cnt = 0; // Reset overflow counter after a successful capture event
}


void TACHO_handleOverflowEvent(void)
{
    __ovf_cnt++;

    if (__ovf_cnt > TACHO_MAX_OVF_COUNT * 2)
    {
        __is_first_cap = 0;
        __ovf_cnt = 2 * TACHO_MAX_OVF_COUNT;
        __rpm = 0; // If there are too many overflows, set RPM to 0
    }
}

__WEAK void TACHO_hwInit(void)
{
}


__WEAK void TACHO_hwStart(void)
{
}


__WEAK void TACHO_hwStop(void)
{
}


__WEAK void TACHO_hwReset(void)
{
}


void TACHO_IN_TriggerCallback(uint16_t cap_cnt)
{
    TACHO_handleCaptureEvent(cap_cnt);
}


void TACHO_TIM_UpdateCallback(void)
{
    TACHO_handleOverflowEvent();
}

/************* (C) COPYRIGHT South China Univ. of Tech. ****** END OF FILE ****/
