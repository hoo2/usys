/*
 * \file syscalls.h
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

#ifndef __syscalls_h__
#define __syscalls_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __weak
#define __weak __attribute__ ((weak))
#endif

/*
 * =========== includes =============
 */

// system includes
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#ifdef __cplusplus
}
#endif

#endif // #ifndef __syscalls_h__
