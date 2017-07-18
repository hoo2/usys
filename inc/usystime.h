/*
 * \file usystime.h
 * \brief
 *    micro system's time and cron functionality implementation
 * Provides:
 *    time(), clock(), cron(), ...
 * \note
 *    This module provides SysTick_Callback(). In order to work the
 *    User has to implement a SysTick_IRQ and call this function
 *
 * This file is part of usys
 *
 * Copyright (C) 2016 Choutouridis Christos (http://www.houtouridis.net)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __usystime_h__
#define __usystime_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <limits.h>

/*
 * ======= User defines =============
 */
#define USYS_CRONTAB_ENTRIES      (10)

/*
 * ===== Data types ========
 */

/*
 * Also defined in types.h
 */
#ifndef  _CLOCK_T_
#define  _CLOCK_T_   unsigned long     /* clock() */
typedef _CLOCK_T_ clock_t;             /*!< CPU time type */
#endif
#ifndef _TIME_T_
#define  _TIME_T_ long                 /* time() */
typedef _TIME_T_ time_t;               /*!< date/time in unix secs past 1-Jan-70 type for 68 years*/
#endif

/*!
 * To read signed CPU time use \ref sclock(). Using sclock() insteed of clock()
 * can help the user program to find out when the cpu clock variable rolls over max value.
 *
 * for ex:
 *
 *    // wait 10 __sticks;
 *    sclock_t mark = sclock(), diff, n;
 *    do {
 *       n = sclock();
 *       diff = (n > mark ) ?
 *          n-mark :                      // if not rolled
 *          0xFFFFFFFF - (n-mark) + 1 ;   // if rolled
 *    } while (diff < 10);
 *
 * \note
 *    DO NOT exceed \ref _SCLOCK_T_MAX_VALUE_ value for this kind of calculations
 *
 * \sa _SCLOCK_DIFF Macro.
 */
typedef long   sclock_t;

/*
 * =========== Helper macros ===============
 */

#define _CLOCK_T_MAX_VALUE_            (ULONG_MAX) //!< Helper macro for maximum signed CPU time calculations
#define _SCLOCK_T_MAX_VALUE_           (LONG_MAX)  //!< Helper macro for maximum signed CPU time calculations
#define _SCLOCK_T_MIN_VALUE_           (LONG_MIN)  //!< Helper macro for minimum signed CPU time calculations


/*!
 *  Calculate the positive time difference of _t2_ and _t1_, where
 *  _t1_, _t2_ are clock_t values
 *  \note
 *    _t2_ is AFTER _t1_
 *
 *  ex:
 *  0   1   2   3   4   5   6   7   8   9
 *      ^                       ^
 *      |                       |
 *      a                       b
 *
 * if : t1=a, t2=b      then  dt = b-a             = t2 - t1
 * if : t1=b, t2=a      then  dt = 9 - (b-a) + 1   = UMAX - (t1-t2) + 1
 *
 */
#define _CLOCK_DIFF(_t2_, _t1_)     ( ((_t2_)>(_t1_)) ? ((_t2_)-(_t1_)) : (_CLOCK_T_MAX_VALUE_ - ((_t1_) - (_t2_)) + 1) )

/*!
 *  Calculate the positive time difference of _t2_ and _t1_, where
 *  _t1_, _t2_ are sclock_t values
 *  \note
 *    _t2_ is AFTER _t1_
 *
 *  ex:
 * -5  -4  -3  -2  -1   0   1   2   3   4
 *      ^                       ^
 *      |                       |
 *      a                       b
 *
 * if : t1=a, t2=b      then  dt = b-a             = t2 - t1
 * if : t1=b, t2=a      then  dt = 9 - (b-a) + 1   = (LMAX-LMIN) - (t1-t2) + 1
 */
#define _SCLOCK_DIFF(_t2_, _t1_)         ( ((_t2_)>(_t1_)) ? ((_t2_)-(_t1_)) : ((_SCLOCK_T_MAX_VALUE_ - _SCLOCK_T_MIN_VALUE_) - ((_t1_) - (_t2_)) + 1) )

/*
 * CPU time macros
 */
#define  usys_msec(_ms_)      (((_ms_) * get_freq()) / 1000)
#define  usys_sec(_s_)        ((_s_) * get_freq())

typedef void (*cronfun_t) (void);            /*!< Pointer to void function (void) to use with cron */
typedef time_t (*ext_time_ft) (time_t *);    /*!< Pointer type for External time function. */
typedef int (*ext_settime_ft) (const time_t *); /*!< Pointer type for External set time function. */

/*!
 * Cron Table data type
 */
typedef struct {
   cronfun_t   fun;
   clock_t     tic;
}crontab_t;


/*
 * ========= Set Functions ============
 */
void usys_set_rtc_time (ext_time_ft f);
void usys_set_rtc_settime (ext_settime_ft f);

/*
 * ======== OS like Functionalities ============
 */
void SysTick_Callback (void);

/*
 * extern declarations (from a HAL or Driver)
 */
extern clock_t get_freq (void);
extern int set_freq (clock_t sf);

clock_t clock (void);
clock_t setclock (clock_t c);

sclock_t sclock (void);
sclock_t setsclock (sclock_t c);

time_t time (time_t *timer);
int settime (const time_t *t);

void service_add (cronfun_t pfun, clock_t interval);
void service_rem (cronfun_t pfun);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __usystime_h__
