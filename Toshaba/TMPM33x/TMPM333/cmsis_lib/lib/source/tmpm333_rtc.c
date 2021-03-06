/**
 *******************************************************************************
 * @file    tmpm333_rtc.c
 * @brief   This file provides API functions for RTC driver.
 * @version V2.1.0
 * @date    2010/08/02
 * 
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". TOSHIBA
 * CORPORATION MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. TOSHIBA CORPORATION
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 * 
 * TOSHIBA CORPORATION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 * 
 * (C)Copyright TOSHIBA CORPORATION 2010 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm333_rtc.h"

#if defined(__TMPM330_RTC_H) || defined(__TMPM332_RTC_H) || defined(__TMPM333_RTC_H)
/** @addtogroup TX03_Periph_Driver
  * @{
  */

/** @defgroup RTC 
  * @brief RTC driver modules
  * @{
  */

/** @defgroup RTC_Private_Defines
  * @{
  */

#define PAGER_PAGE_SET         ((uint8_t)0x01)
#define PAGER_PAGE_CLEAR       ((uint8_t)0xEE)
#define PAGER_ADJUST_SET       ((uint8_t)0x10)
#define PAGER_ADJUST_CLEAR     ((uint8_t)0xEF)
#define PAGER_ENATMR_SET       ((uint8_t)0x08)
#define PAGER_ENATMR_CLEAR     ((uint8_t)0xE7)
#define PAGER_ENAALM_SET       ((uint8_t)0x04)
#define PAGER_ENAALM_CLEAR     ((uint8_t)0xEB)
#define PAGER_INTENA_SET       ((uint8_t)0x80)
#define PAGER_INTENA_CLEAR     ((uint8_t)0x6F)

#define RESTR_DIS_CLEAR        ((uint8_t)0x0F)
#define RESTR_DIS1HZ_SET       ((uint8_t)0x80)
#define RESTR_DIS16HZ_SET      ((uint8_t)0x40)
#define RESTR_RSTTMR_SET       ((uint8_t)0x20)
#define RESTR_RSTTMR_CLEAR     ((uint8_t)0xCF)
#define RESTR_RSTALM_SET       ((uint8_t)0x10)
#define RESTR_RSTALM_CLEAR     ((uint8_t)0xCF)

#define HOURR_HO5_SET          ((uint8_t)0x20)
#define HOURR_HO5_CLEAR        ((uint8_t)0xDF)

/** @} */
/* End of group RTC_Private_Defines */


/** @defgroup RTC_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group RTC_Private_FunctionPrototypes */

/** @defgroup RTC_Private_Functions
  * @{
  */

/** @} */
/* End of group group RTC_Private_Functions */

/** @defgroup RTC_Exported_Functions
  * @{
  */

/**
  * @brief  Set the RTC SECR register.
  * @param  Sec: New sec value, max 59
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...59 
  * @retval None
  */
void RTC_SetSec(uint8_t Sec)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_SECOND(Sec));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Sec / 10U) * 16U + Sec % 10U);
    /* Write sec value into SECR register */
    TSB_RTC->SECR = tmp;
}

/**
  * @brief  Get the sec value.
  * @param  None
  * @retval The sec value.
  */
uint8_t RTC_GetSec(void)
{
    uint8_t sec = 0U;
    uint8_t tmp = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    do {
        tmp = TSB_RTC->SECR;
        sec = TSB_RTC->SECR;
    }
    while (tmp != sec);

    sec = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return second value  */
    return sec;
}

/**
  * @brief  Set the RTC MINR register.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Min: New min value, max 59
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...59 
  * @retval None
  */
void RTC_SetMin(RTC_FuncMode NewMode, uint8_t Min)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_MINUTE(Min));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Min / 10U) * 16U + Min % 10U);
    /* Write min value into MINR register */
    TSB_RTC->MINR = tmp;
}

/**
  * @brief  Get the min value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The min value.
  */
uint8_t RTC_GetMin(RTC_FuncMode NewMode)
{
    uint8_t min = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        do {
            tmp = TSB_RTC->MINR;
            min = TSB_RTC->MINR;
        }
        while (tmp != min);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->MINR;
    }

    min = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return minute value  */
    return min;
}

/**
  * @brief  Get AM or PM state in the 12 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE
  * @retval mode: The mode of the time.
  *   This parameter can be one of the following values:
  *   RTC_AM_MODE, RTC_PM_MODE 
  */
uint8_t RTC_GetAMPM(RTC_FuncMode NewMode)
{
    uint8_t tmp = 0U;
    uint8_t mode = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        do {
            tmp = TSB_RTC->HOURR;
            mode = TSB_RTC->HOURR;
        }
        while (tmp != mode);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->HOURR;
    }

    tmp &= HOURR_HO5_SET;

    if (tmp == HOURR_HO5_SET) {
        mode = RTC_PM_MODE;
    } else {
        mode = RTC_AM_MODE;
    }

    return (mode);
}

/**
  * @brief  Set the RTC HOURR register in the 24 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Hour: New hour value, max 23
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...23 
  * @retval None
  */
void RTC_SetHour24(RTC_FuncMode NewMode, uint8_t Hour)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_24(Hour));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Hour / 10U) * 16U + Hour % 10U);
    /* Write hour value into HOURR register */
    TSB_RTC->HOURR = tmp;
}

/**
  * @brief  Set the RTC HOURR register in the 12 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Hour: New hour value, max 11
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...11
  * @param  AmPm: New time mode
  *   This parameter can be one of the following values:
  *   RTC_AM_MODE or RTC_PM_MODE
  * @retval None
  */
void RTC_SetHour12(RTC_FuncMode NewMode, uint8_t Hour, uint8_t AmPm)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_12(Hour));
    assert_param(IS_RTC_FUNC_MODE(NewMode));
    assert_param(IS_RTC_AMPM_MODE(AmPm));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Hour / 10U) * 16U + Hour % 10U);

    if (AmPm == RTC_PM_MODE) {
        tmp |= HOURR_HO5_SET;
    } else {
        /* Do nothing */
    }

    /* Write hour value into HOURR register */
    TSB_RTC->HOURR = tmp;
}

/**
  * @brief  Get the hour value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The hour value.
  */
uint8_t RTC_GetHour(RTC_FuncMode NewMode)
{
    uint8_t HourMode = 0U;
    uint8_t hour = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    /* Get hour mode */
    HourMode = RTC_GetHourMode();
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get hour value */
        do {
            tmp = TSB_RTC->HOURR;
            hour = TSB_RTC->HOURR;
        }
        while (tmp != hour);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->HOURR;
    }

    if (HourMode == RTC_12_HOUR_MODE) {
        tmp &= HOURR_HO5_CLEAR;
    } else {
        /* Do nothing */
    }

    hour = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return hour value  */
    return hour;
}

/**
  * @brief  Set the RTC DAYR register.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Day: New day value
  *   This parameter can be one of the following values:
  *   RTC_SUN, RTC_MON, RTC_TUE, RTC_WED, RTC_THU, RTC_FRI, RTC_SAT 
  * @retval None
  */
void RTC_SetDay(RTC_FuncMode NewMode, uint8_t Day)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_DAY(Day));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    /* Write day value into DAYR register */
    TSB_RTC->DAYR = Day;
}

/**
  * @brief  Get the day value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The day value.
  */
uint8_t RTC_GetDay(RTC_FuncMode NewMode)
{
    uint8_t day = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get day value */
        do {
            tmp = TSB_RTC->DAYR;
            day = TSB_RTC->DAYR;
        }
        while (tmp != day);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        day = TSB_RTC->DAYR;
    }

    /* return day value  */
    return day;
}

/**
  * @brief  Set the RTC DATER register.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Date: New date value, Max 31
  *   This parameter can be one of the following values:
  *   1, 2, ...31 
  * @retval None
  */
void RTC_SetDate(RTC_FuncMode NewMode, uint8_t Date)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_DATE(Date));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Date / 10U) * 16U + Date % 10U);
    /* Write date value into DAYR register */
    TSB_RTC->DATER = tmp;
}

/**
  * @brief  Get the date value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The date value.
  */
uint8_t RTC_GetDate(RTC_FuncMode NewMode)
{
    uint8_t tmp = 0U;
    uint8_t date = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get date value */
        do {
            tmp = TSB_RTC->DATER;
            date = TSB_RTC->DATER;
        }
        while (tmp != date);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->DATER;
    }

    date = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return day value  */
    return date;
}

/**
  * @brief  Set the RTC MONTHR register.
  * @param  Month: New month value, Max 12
  *   This parameter can be one of the following values:
  *   1, 2, 3, ...12 
  * @retval None
  */
void RTC_SetMonth(uint8_t Month)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_MONTH(Month));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Month / 10U) * 16U + Month % 10U);
    /* Write month value into MONTHR register */
    TSB_RTC->MONTHR = tmp;
}

/**
  * @brief  Get the month value.
  * @param  None
  * @retval The month value.
  */
uint8_t RTC_GetMonth(void)
{
    uint8_t tmp = 0U;
    uint8_t month = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    /* Get month value */
    do {
        tmp = TSB_RTC->MONTHR;
        month = TSB_RTC->MONTHR;
    }
    while (tmp != month);

    month = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return month value */
    return month;
}

/**
  * @brief  Set the RTC YEARR register.
  * @param  Year: New year value, Max 99
  *   This parameter can be one of the following values:
  *   0, 1, 2, 3, ...99
  * @retval None
  */
void RTC_SetYear(uint8_t Year)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_YEAR(Year));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Year / 10U) * 16U + Year % 10U);
    /* Write year value into YEARR register */
    TSB_RTC->YEARR = tmp;
}

/**
  * @brief  Get the year value.
  * @param  None
  * @retval The year value.
  */
uint8_t RTC_GetYear(void)
{
    uint8_t tmp = 0U;
    uint8_t year = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    /* Get year value */
    do {
        tmp = TSB_RTC->YEARR;
        year = TSB_RTC->YEARR;
    }
    while (tmp != year);

    year = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return year value  */
    return year;
}

/**
  * @brief  Select 24-hour clock or 12-hour clock.
  * @param  HourMode: Select 24-hour clock or 12-hour clock 
  *   This parameter can be one of the following values:
  *   RTC_12_HOUR_MODE, RTC_24_HOUR_MODE
  * @retval None
  */
void RTC_SetHourMode(uint8_t HourMode)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_MODE(HourMode));

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Set MONTHR<MO0> to select 24-hour mode/12-hour mode */
    if (HourMode == RTC_12_HOUR_MODE) {
        TSB_RTC_MONTHR_MO0 = (uint32_t) RTC_12_HOUR_MODE;
    } else {
        TSB_RTC_MONTHR_MO0 = (uint32_t) RTC_24_HOUR_MODE;
    }
}

/**
  * @brief  Get hour mode.
  * @param  None
  * @retval mode: The mode of the hour.
  *   This parameter can be one of the following values:
  *   RTC_12_HOUR_MODE, RTC_24_HOUR_MODE 
  */
uint8_t RTC_GetHourMode(void)
{
    uint8_t mode = 0U;

    /* Select PAGE1 */
    mode = TSB_RTC->PAGER;
    mode &= PAGER_PAGE_CLEAR;
    mode |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = mode;

    if (TSB_RTC_MONTHR_MO0 == 1U) {
        mode = RTC_24_HOUR_MODE;
    } else {
        mode = RTC_12_HOUR_MODE;
    }

    return (mode);
}

/**
  * @brief  Set leap-year state.
  * @param  LeapYear: Set leap-year 
  *   This parameter can be one of the following values:
  *   RTC_LEAP_YEAR_0, RTC_LEAP_YEAR_1,
  *   RTC_LEAP_YEAR_2 or RTC_LEAP_YEAR_3 
  * @retval None
  */
void RTC_SetLeapYear(uint8_t LeapYear)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_LEAP_YEAR(LeapYear));

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Set <LEAP1> and <LEAP0> to select leap year state */
    TSB_RTC->YEARR = LeapYear;
}

/**
  * @brief  Get leap-year state from YEARR register.
  * @param  None 
  * @retval The state of leap year.
  */
uint8_t RTC_GetLeapYear(void)
{
    uint8_t tmp = 0U;
    uint8_t leapyear = 0U;

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Get leap year state */
    do {
        tmp = TSB_RTC->YEARR;
        leapyear = TSB_RTC->YEARR;
    }
    while (tmp != leapyear);

    /* return leap year state  */
    return (leapyear);
}

/**
  * @brief  Set time adjustment + or - 30 seconds.
  * @param  None
  * @retval None
  */
void RTC_SetTimeAdjustReq(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ADJUST> */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ADJUST_CLEAR;
    tmp |= PAGER_ADJUST_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Get request state for time adjust.
  * @param  None
  * @retval The state of time adjustment.
  * This parameter can be one of the following values:
  * RTC_REQ or RTC_NO_REQ
  */
RTC_ReqState RTC_GetTimeAdjustReq(void)
{
    uint8_t tmp0 = 0U;
    RTC_ReqState tmp1 = RTC_NO_REQ;

    tmp0 = TSB_RTC->PAGER;
    tmp0 &= PAGER_ADJUST_SET;

    if (tmp0 == PAGER_ADJUST_SET) {
        tmp1 = RTC_REQ;
    } else {
        tmp1 = RTC_NO_REQ;
    }

    return (tmp1);
}

/**
  * @brief  Enable RTC clock function.
  * @param  None
  * @retval None
  */
void RTC_EnableClock(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ENATMR> to enable clock function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENATMR_CLEAR;
    tmp |= PAGER_ENATMR_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Disable RTC clock function.
  * @param  None
  * @retval None
  */
void RTC_DisableClock(void)
{
    uint8_t tmp = 0U;

    /* Clear PAGER<ENATMR> to disable clock function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENATMR_CLEAR;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Enable RTC alarm function.
  * @param  None
  * @retval None
  */
void RTC_EnableAlarm(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ENAALM> to enable alarm function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENAALM_CLEAR;
    tmp |= PAGER_ENAALM_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Disable RTC alarm function.
  * @param  None
  * @retval None
  */
void RTC_DisableAlarm(void)
{
    uint8_t tmp = 0U;

    /* Clear PAGER<ENAALM> to disable alarm function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENAALM_CLEAR;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Enable or disable INTRTC.
  * @param  NewState: New state of RTCINT.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void RTC_SetRTCINT(FunctionalState NewState)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set PAGER<INTENA> to enable or disable INTRTC */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_INTENA_CLEAR;

    if (NewState == ENABLE) {
        tmp |= PAGER_INTENA_SET;
    } else {
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Set output signals from ALARM pin.
  * @param  Output: Set ALARM pin output.
  *   This parameter can be RTC_LOW_LEVEL, RTC_PULSE_1_HZ
  *   or RTC_PULSE_16_HZ.
  * @retval None
  */
void RTC_SetAlarmOutput(uint8_t Output)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_ALARM_OUTPUT(Output));

    switch (Output) {
    case RTC_LOW_LEVEL:
        /* Set PAGER<ENAALM> = 1 to enable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        tmp |= PAGER_ENAALM_SET;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS1HZ> = 1 and <DIS16HZ> = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS1HZ_SET | RESTR_DIS16HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_1_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS1HZ> = 0 and <DIS16HZ> = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= RESTR_DIS16HZ_SET;
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_16_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS1HZ> = 1 and <DIS16HZ> = 0  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= RESTR_DIS1HZ_SET;
        TSB_RTC->RESTR = tmp;
        break;
    default:
        /* do nothing */
        break;
    }
}

/**
  * @brief  Reset RTC clock second counter.
  * @param  None
  * @retval None
  */
void RTC_ResetClockSec(void)
{
    uint8_t tmp = 0U;

    /* Set RESTR<RSTTMR> = 1 to reset clock sec counter */
    tmp = TSB_RTC->RESTR;
    tmp &= RESTR_RSTTMR_CLEAR;
    tmp |= RESTR_RSTTMR_SET;
    TSB_RTC->RESTR = tmp;
}

/**
  * @brief  Get request state for reset RTC clock second counter.
  * @param  None
  * @retval The state of reset clock request.
  * This parameter can be one of the following values:
  * RTC_REQ or RTC_NO_REQ
  */
RTC_ReqState RTC_GetResetClockSecReq(void)
{
    uint8_t tmp0 = 0U;
    RTC_ReqState tmp1 = RTC_NO_REQ;

    tmp0 = TSB_RTC->RESTR;
    tmp0 &= RESTR_RSTTMR_SET;

    if (tmp0 == RESTR_RSTTMR_SET) {
        tmp1 = RTC_REQ;
    } else {
        tmp1 = RTC_NO_REQ;
    }

    return (tmp1);
}

/**
  * @brief  Reset RTC alarm.
  * @param  None
  * @retval None
  */
void RTC_ResetAlarm(void)
{
    uint8_t tmp = 0U;

    /* Set RESTR<RSTALM> = 1 to reset alarm */
    tmp = TSB_RTC->RESTR;
    tmp &= RESTR_RSTALM_CLEAR;
    tmp |= RESTR_RSTALM_SET;
    TSB_RTC->RESTR = tmp;
}

/**
  * @brief  Set the RTC date.
  * @param  DateStruct: The structure containing basic Date configuration.
  * @retval None
  */
void RTC_SetDateValue(RTC_DateTypeDef * DateStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_RTC_LEAP_YEAR(DateStruct->LeapYear));
    assert_param(IS_RTC_YEAR(DateStruct->Year));
    assert_param(IS_RTC_MONTH(DateStruct->Month));
    assert_param(IS_RTC_DATE(DateStruct->Date));
    assert_param(IS_RTC_DAY(DateStruct->Day));

    /* Set LeapYear */
    RTC_SetLeapYear(DateStruct->LeapYear);
    /* Set year value */
    RTC_SetYear(DateStruct->Year);
    /* Set month value */
    RTC_SetMonth(DateStruct->Month);
    /* Set date value */
    RTC_SetDate(RTC_CLOCK_MODE, DateStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_CLOCK_MODE, DateStruct->Day);
}

/**
  * @brief  Get the RTC date.
  * @param  DateStruct: The structure containing basic Date configuration will be modified.
  * @retval None
  */
void RTC_GetDateValue(RTC_DateTypeDef * DateStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));

    /* Get LeapYear */
    DateStruct->LeapYear = RTC_GetLeapYear();
    /* Get year value */
    DateStruct->Year = RTC_GetYear();
    /* Get month value */
    DateStruct->Month = RTC_GetMonth();
    /* Get date value */
    DateStruct->Date = RTC_GetDate(RTC_CLOCK_MODE);
    /* Get day value */
    DateStruct->Day = RTC_GetDay(RTC_CLOCK_MODE);
}

/**
  * @brief  Set the RTC time.
  * @param  TimeStruct: The structure containing basic Time configuration.
  * @retval None
  */
void RTC_SetTimeValue(RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));
    assert_param(IS_RTC_HOUR_MODE(TimeStruct->HourMode));

    if (TimeStruct->HourMode) {
        assert_param(IS_RTC_HOUR_24(TimeStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(TimeStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(TimeStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(TimeStruct->Min));
    assert_param(IS_RTC_SECOND(TimeStruct->Sec));

    /* Set hour mode */
    RTC_SetHourMode(TimeStruct->HourMode);

    /* Set hour value */
    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        RTC_SetHour24(RTC_CLOCK_MODE, TimeStruct->Hour);
    } else {
        RTC_SetHour12(RTC_CLOCK_MODE, TimeStruct->Hour, TimeStruct->AmPm);
    }

    /* Set minute value */
    RTC_SetMin(RTC_CLOCK_MODE, TimeStruct->Min);
    /* Set second value */
    RTC_SetSec(TimeStruct->Sec);
}

/**
  * @brief  Get the RTC time.
  * @param  TimeStruct: The structure containing basic Time configuration will be modified.
  * @retval None
  */
void RTC_GetTimeValue(RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));

    /* Get hour mode */
    TimeStruct->HourMode = RTC_GetHourMode();
    /* Get hour value */
    TimeStruct->Hour = RTC_GetHour(RTC_CLOCK_MODE);

    if (TimeStruct->HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        TimeStruct->AmPm = RTC_GetAMPM(RTC_CLOCK_MODE);
    } else {
        TimeStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    TimeStruct->Min = RTC_GetMin(RTC_CLOCK_MODE);
    /* Get second value */
    TimeStruct->Sec = RTC_GetSec();
}

/**
  * @brief  Set the RTC date and time.
  * @param  DateStruct: The structure containing basic Date configuration.
  * @param  TimeStruct: The structure containing basic Time configuration.
  * @retval None
  */
void RTC_SetClockValue(RTC_DateTypeDef * DateStruct, RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));
    assert_param(IS_RTC_LEAP_YEAR(DateStruct->LeapYear));
    assert_param(IS_RTC_YEAR(DateStruct->Year));
    assert_param(IS_RTC_MONTH(DateStruct->Month));
    assert_param(IS_RTC_DATE(DateStruct->Date));
    assert_param(IS_RTC_DAY(DateStruct->Day));
    assert_param(IS_RTC_HOUR_MODE(TimeStruct->HourMode));

    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        assert_param(IS_RTC_HOUR_24(TimeStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(TimeStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(TimeStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(TimeStruct->Min));
    assert_param(IS_RTC_SECOND(TimeStruct->Sec));

    /* Set LeapYear */
    RTC_SetLeapYear(DateStruct->LeapYear);
    /* Set year value */
    RTC_SetYear(DateStruct->Year);
    /* Set month value */
    RTC_SetMonth(DateStruct->Month);
    /* Set date value */
    RTC_SetDate(RTC_CLOCK_MODE, DateStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_CLOCK_MODE, DateStruct->Day);
    /* Set hour mode */
    RTC_SetHourMode(TimeStruct->HourMode);

    /* Set hour value */
    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        RTC_SetHour24(RTC_CLOCK_MODE, TimeStruct->Hour);
    } else {
        RTC_SetHour12(RTC_CLOCK_MODE, TimeStruct->Hour, TimeStruct->AmPm);
    }

    /* Set minute value */
    RTC_SetMin(RTC_CLOCK_MODE, TimeStruct->Min);
    /* Set second value */
    RTC_SetSec(TimeStruct->Sec);
}

/**
  * @brief  Get the RTC date and time.
  * @param  DateStruct: The structure containing basic Date configuration will be modified.
  * @param  TimeStruct: The structure containing basic Time configuration will be modified.
  * @retval None
  */
void RTC_GetClockValue(RTC_DateTypeDef * DateStruct, RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));

    /* Get LeapYear */
    DateStruct->LeapYear = RTC_GetLeapYear();
    /* Get year value */
    DateStruct->Year = RTC_GetYear();
    /* Get month value */
    DateStruct->Month = RTC_GetMonth();
    /* Get date value */
    DateStruct->Date = RTC_GetDate(RTC_CLOCK_MODE);
    /* Get day value */
    DateStruct->Day = RTC_GetDay(RTC_CLOCK_MODE);
    /* Get hour mode */
    TimeStruct->HourMode = RTC_GetHourMode();
    /* Get hour value */
    TimeStruct->Hour = RTC_GetHour(RTC_CLOCK_MODE);

    if (TimeStruct->HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        TimeStruct->AmPm = RTC_GetAMPM(RTC_CLOCK_MODE);
    } else {
        TimeStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    TimeStruct->Min = RTC_GetMin(RTC_CLOCK_MODE);
    /* Get second value */
    TimeStruct->Sec = RTC_GetSec();
}

/**
  * @brief  Set the RTC alarm date and time.
  * @param  AlarmStruct: The structure containing basic alarm configuration.
  * @retval None
  */
void RTC_SetAlarmValue(RTC_AlarmTypeDef * AlarmStruct)
{
    uint8_t HourMode;
    HourMode = RTC_GetHourMode();

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(AlarmStruct));
    assert_param(IS_RTC_DATE(AlarmStruct->Date));
    assert_param(IS_RTC_DAY(AlarmStruct->Day));

    if (HourMode == RTC_24_HOUR_MODE) {
        assert_param(IS_RTC_HOUR_24(AlarmStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(AlarmStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(AlarmStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(AlarmStruct->Min));

    /* Set date value */
    RTC_SetDate(RTC_ALARM_MODE, AlarmStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_ALARM_MODE, AlarmStruct->Day);

    /* Set hour value */
    if (HourMode == RTC_12_HOUR_MODE) {
        RTC_SetHour12(RTC_ALARM_MODE, AlarmStruct->Hour, AlarmStruct->AmPm);
    } else {
        RTC_SetHour24(RTC_ALARM_MODE, AlarmStruct->Hour);
    }

    /* Set minute value */
    RTC_SetMin(RTC_ALARM_MODE, AlarmStruct->Min);
}

/**
  * @brief  Get the RTC alarm date and time.
  * @param  AlarmStruct: The structure containing basic alarm configuration will be modified.
  * @retval None
  */
void RTC_GetAlarmValue(RTC_AlarmTypeDef * AlarmStruct)
{
    uint8_t HourMode;

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(AlarmStruct));

    HourMode = RTC_GetHourMode();

    /* Get date value */
    AlarmStruct->Date = RTC_GetDate(RTC_ALARM_MODE);
    /* Get day value */
    AlarmStruct->Day = RTC_GetDay(RTC_ALARM_MODE);
    /* Get hour value */
    AlarmStruct->Hour = RTC_GetHour(RTC_ALARM_MODE);

    if (HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        AlarmStruct->AmPm = RTC_GetAMPM(RTC_ALARM_MODE);
    } else {
        AlarmStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    AlarmStruct->Min = RTC_GetMin(RTC_ALARM_MODE);
}

/** @} */
/* End of group RTC_Exported_Functions */

/** @} */
/* End of group RTC */

/** @} */
/* End of group TX03_Periph_Driver */
#endif                          /* defined (__TMPM330_RTC_H) || (__TMPM332_RTC_H) || (__TMPM333_RTC_H) */
