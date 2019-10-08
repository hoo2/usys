/*
 * \file syscalls.c
 * \brief
 *    Syscalls decelerations to link with libc, nanolib, newlib
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

/* Includes */
#include <syscalls.h>


/* Variables */
#undef errno
extern int32_t errno;

uint8_t *__env[1] = { 0 };
uint8_t **environ = __env;


/* Functions */
#define  __use_1(x)           (void)(x)
#define  __use_2(x, y)        (void)(x); (void)(y)
#define  __use_3(x, y, z)     (void)(x); (void)y; (void)(z)
#define  __use_4(x, y, z, w)  (void)(x); (void)y; (void)(z); (void)(w)

#define  __not_implemented() { \
   errno = ENOSYS;             \
   return -1;                  \
}


void initialise_monitor_handles() {
}

void _exit (int32_t status) {
   __use_1 (status);
   while (1) {}      /* Make sure we hang here */
}

/* Implement your write code here, this is used by puts and printf for example */
/* return len; */
__weak int _write (int32_t file, uint8_t *ptr, int32_t len) {
   __use_3(file, *ptr, len);
   __not_implemented();
}

void * _sbrk(int32_t incr) {
	extern unsigned long   _ebss;    /* Set by linker.  */
	static char * heap_start = 0;
	char * ret;

	if (heap_start == 0) {
	   // Run the first time only to initialize heap_start
	   heap_start = (char *)(&_ebss + sizeof (unsigned long));
	}

	// Return the entry value of heap_start
	ret = heap_start;
	heap_start += incr;

	return (void *) ret;
}

__weak int _getpid(void) { __not_implemented(); }
__weak int _gettimeofday(struct timeval  *ptimeval, void *ptimezone) {
   __use_2(*ptimeval, ptimezone);
   __not_implemented();
}
__weak int _kill(int32_t pid, int32_t sig)  {
   __use_2(pid, sig);
   __not_implemented();
}
__weak int _close(int32_t file) { __use_1(file); __not_implemented(); }
__weak int _fstat(int32_t file, struct stat *st) { __use_2(file, *st); __not_implemented(); }
__weak int _isatty(int32_t file) { __use_1(file); __not_implemented(); }
__weak int _lseek(int32_t file, int32_t ptr, int32_t dir) {
   __use_3(file, ptr, dir);
   __not_implemented();
}
__weak int _read(int32_t file, uint8_t *ptr, int32_t len) {
   __use_3(file, *ptr, len);
   __not_implemented();
}
__weak int _readlink(const char *path, char *buf, size_t bufsize) {
   __use_3(*path, *buf, bufsize);
   __not_implemented();
}
__weak int _open(const uint8_t *path, int32_t flags, int32_t mode) {
   __use_3 (*path, flags, mode);
   __not_implemented();
}
__weak int _wait(int32_t *status) { __use_1 (*status); __not_implemented(); }
__weak int _unlink(const uint8_t *name) { __use_1(*name); __not_implemented(); }
__weak int _times(struct tms *buf) { __use_1(*buf); __not_implemented(); }
__weak int _stat(const uint8_t *file, struct stat *st) {
   __use_2(*file, *st);
   __not_implemented();
}
__weak int _symlink(const char *path1, const char *path2)  {
   __use_2(*path1, *path2);
   __not_implemented();
}
__weak int _link(const uint8_t *old, const uint8_t *new) {
   __use_2(*old, *new);
   __not_implemented();
}
__weak int _fork(void) { __not_implemented(); }
__weak int _execve(const uint8_t *name, uint8_t * const *argv, uint8_t * const *env) {
   __use_3 (*name, *argv, *env);
   __not_implemented();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
   /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

   /* Infinite loop */
   _exit (0);
}
#endif

/*!
 * Minimal __assert_func used by the assert() macro
 */
void __assert_func (const char *file, int line, const char *func, const char *failedexpr) {
   __use_4(*file, line, *func, *failedexpr);
   _exit (0);
}

/*!
 * Minimal __assert() uses __assert__func()
 */
void __assert(const char *file, int line, const char *failedexpr) {
   __assert_func (file, line, NULL, failedexpr);
}
