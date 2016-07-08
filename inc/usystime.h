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
#ifndef __systime_h__
#define __systime_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <limits.h>

/*
 * ======= User defines =============
 */
#define ST_MAX_CRONTAB_ENTRIES      (10)

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

/*
 * To read signed CPU time use \sa sclock(). Using this can help the
 * user program to find out when the cpu clock variable rolls over max value.
 * for ex:
 *    // wait 10 __sticks;
 *    sclock_t mark = sclock(), diff, n;
 *    do {
 *       n = sclock();
 *       diff = ((n - mark) >=0 ) ? n-mark : -n-mark;
 *       // (n-mark) if not rolled, (-n-mark) if rolled
 *    } while (diff < 10);
 * \note
 *    DO NOT exceed \sa _SCLOCK_T_MAX_VALUE_ value for this kind of calculations
 */
typedef long   sclock_t;

/*
 * =========== Helper macros ===============
 */

#define _CLOCK_T_MAX_VALUE_            (ULONG_MAX) //!< Helper macro for maximum signed CPU time calculations
#define _SCLOCK_T_MAX_VALUE_           (LONG_MAX)  //!< Helper macro for maximum signed CPU time calculations

//! Absolute value macro
#define _SC_ABS(_x)                    ( (_x<0) ? (-_x) : (_x) )



/*!
 *  Calculate the positive time difference of _t2 and _t1, where
 *  _t1, _t2 are clock_t values
 *  \note
 *    _t2 is AFTER _t1
 */
#define _CLOCK_DIFF(_t2, _t1)          ( ((_t2-_t1) >= 0) ? (_t2-_t1) : (_CLOCK_T_MAX_VALUE_ - _t1 + _t2 +1) )

/*!
 *  Calculate the positive time difference of _t2 and _t1, where
 *  _t1, _t2 are sclock_t values
 *  \note
 *    _t2 is AFTER _t1
 */
#define _SCLOCK_DIFF(_t2, _t1)         ( ((_t2-_t1) >= 0) ? (_t2-_t1) : _SC_ABS(-_t2-_t1) )


/*!
 * Pointer to void function (void) to use with cron
 */
typedef void (*cronfun_t) (void);

/*!
 * Cron Table data type
 */
typedef struct {
   cronfun_t   fun;
   clock_t     tic;
}crontab_t;

/*
 * extern declarations (from a HAL or Driver)
 */
extern clock_t get_freq (void);

/* ======== OS like Functionalities ============ */

/*!
 * \brief
 *    micro-system service.
 *
 * This service implements the SysTick callback function in order
 * to provide micro system - os like functionalities to an application
 * without RTOS
 * \note
 *    The User HAS TO CALL this from the application's SysTick_IRQ
 */
void SysTick_Callback (void);

clock_t clock (void);
clock_t setclock (clock_t c);

sclock_t sclock (void);
sclock_t setsclock (sclock_t c);

time_t time(time_t *timer);
int settime(const time_t *t);

void service_add (cronfun_t pfun, clock_t interval);
void service_rem (cronfun_t pfun);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __systime_h__
