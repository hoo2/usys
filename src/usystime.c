/*
 * \file usystime.c
 * \brief
 *    A target independent micro system implementation
 * Provides:
 *    time(), clock(), ...
 *
 * This file is part of toolbox
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
#include <usystime.h>

/*
 * ================== Static Data =======================
 */
static clock_t volatile __ticks;       //!< CPU time
static sclock_t volatile  __sticks;    //!< signed CPU time
static time_t  volatile __now;         //!< Time in UNIX seconds past 1-Jan-70

static ext_time_ft _ext_time = NULL;         //!< Pointer to External time callback function
static ext_settime_ft _ext_settime = NULL;   //!< Pointer to External set time callback function

/*!<
 * \note
 *    The usys provides a time capability based on \sa __now which updated
 *    via the Systick time base. If the User application need a more accurate
 *    time system, it can use these pointers to link for example with an RTC
 *    time system.
 */

/*!
 *  Cron Table holds all requested entries for periodic function calls.
 *  \note
 *    All the entries will run in privileged mode and will use the main
 *    stack.
 */
static crontab_t  _crontab[USYS_CRONTAB_ENTRIES];


/*!
 * \brief
 *    micro-system time base service for CPU time.
 *
 * This service implements the SysTick callback function in order
 * to provide micro system - os like functionalities to an application
 * without RTOS
 * \note
 *    The User HAS TO CALL this from the application's SysTick_IRQ
 */
void SysTick_Callback (void)
{
   int i;

   // Time
   ++__ticks;
   ++__sticks;
   if ( !_ext_time && !(__ticks % get_freq ()) )
      ++__now; // Do not update __now when we have external time system

   // Cron
   for (i=0 ; i<USYS_CRONTAB_ENTRIES ; ++i) {
      if (_crontab[i].fun && !(__ticks %_crontab[i].tic))
         _crontab[i].fun ();
   }
}
/*
 * ========= Set Functions ============
 */

/*!
 * \brief
 *    Set the External time() provider. This sets the \sa _ext_time
 *    This way the usys can call the _ext_time() to provide a more
 *    accurate time().
 *
 * \param   f     Pointer to User's (or driver's) callback
 * \return        None
 */
void usys_set_rtc_time (ext_time_ft f) {
   if (f)   _ext_time = f;
}

/*!
 * \brief
 *    Set the External settime() forwarder. This sets the \sa _ext_settime
 *    This way the usys can call the _extset_time() when sets the time.
 *
 * \param   f     Pointer to User's (or driver's) callback
 * \return        None
 */
void usys_set_rtc_settime (ext_settime_ft f) {
   if (f)   _ext_settime = f;
}

/*
 * ======== OS like Functionalities ============
 */

/*!
 * \brief
 *    determines the processor time.
 * \return
 *    the implementation's best approximation to the processor time
 *    used by the program since program invocation. The time in
 *    seconds is the value returned divided by the value of the macro
 *    CLK_TCK or CLOCKS_PER_SEC
 */
inline clock_t clock (void) {
   return (clock_t) __ticks;
}

/*!
 * \brief
 *    Set the processor time used.
 * \param
 *    clk   Pointer to new CPU time value
 * \return
 *    The implementation's best approximation to the processor time
 *    used by the program since program invocation. The time in
 *    seconds is the value returned divided by the value of the macro
 *    CLK_TCK or CLOCKS_PER_SEC
 */
inline clock_t setclock (clock_t c) {
   return __ticks = c;
}

/*!
 * \brief
 *    determines the processor signed time.
 * \return
 *    the implementation's best approximation to the processor signed
 *    time used by the program since program invocation. The time in
 *    seconds is the value returned divided by the value of the macro
 *    CLK_TCK or CLOCKS_PER_SEC
 */
inline sclock_t sclock (void) {
   return (sclock_t) __sticks;
}

/*!
 * \brief
 *    Set the processor signed time.
 * \param
 *    clk   Pointer to new CPU time value
 * \return
 *    The implementation's best approximation to the processor signed
 *    time used by the program since program invocation. The time in
 *    seconds is the value returned divided by the value of the macro
 *    CLK_TCK or CLOCKS_PER_SEC
 */
inline sclock_t setsclock (sclock_t c) {
   return __sticks = c;
}

/*!
 * \brief
 *    determines the current calendar time. The encoding of the value is
 *    unspecified.
 * \return
 *    The implementations best approximation to the current calendar
 *    time. If timer is not a null pointer, the return value
 *    is also assigned to the object it points to.
 */
time_t time (time_t *timer)
{
   if (_ext_time)
      return _ext_time (timer);     // Forward to external time system
   else {                           // If no external system, use __now
      if (timer)  *timer = (time_t)__now;
      return (time_t)__now;
   }
}


/*!
 * \brief
 *    Sets the system's idea of the time and date.  The time,
 *    pointed to by t, is measured in seconds since the Epoch, 1970-01-01
 *    00:00:00 +0000 (UTC).
 * \param
 *    t     Pointer to new system's time and date.
 * \return
 *    On success, zero is returned.  On error, -1 is returned
 */
int settime (const time_t *t)
{
   if (_ext_settime)
      return _ext_settime (t);      // Forward to external time system
   else {                           // If no external system, use __now
      if (t) {
         __now = *t;
         return 0;
      }
      else
         return -1;
   }
}

/*!
 * \brief
 *    Add a function to cron
 *
 * \param   pfun  Pointer to function
 * \param   tic   Tick period. Cron will run the \a pfun every \b tic Ticks
 *
 * \note
 *    All the entries will run in privileged mode and will use the main
 *    stack.
 */
void service_add (cronfun_t pfun, clock_t tic)
{
   uint32_t i;
   for (i=0 ; i<USYS_CRONTAB_ENTRIES ; ++i)
      if (!_crontab[i].fun) {
         _crontab[i].fun = pfun;
         _crontab[i].tic = tic;
         return;
      }
}

/*!
 * \brief
 *    Remove a function from cron
 *
 * \param   pfun Pointer to function
 */
void service_rem (cronfun_t pfun)
{
   int i;
   for (i=0 ; i<USYS_CRONTAB_ENTRIES ; ++i)
      if (_crontab[i].fun == pfun)
         _crontab[i].fun = (void*)0;
}


