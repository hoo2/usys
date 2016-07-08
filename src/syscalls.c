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
#define  __not_implemented() {   \
      errno = ENOSYS;            \
      return -1;                 \
   }


void initialise_monitor_handles() {
}

void _exit(int32_t status) {
   while (1) {}      /* Make sure we hang here */
}

/* Implement your write code here, this is used by puts and printf for example */
/* return len; */
int _write(int32_t file, uint8_t *ptr, int32_t len) { __not_implemented(); }

void * _sbrk(int32_t incr) {
	extern char   end; /* Set by linker.  */
	static char * heap_end;
	char *        prev_heap_end;

	if (heap_end == 0) {
		heap_end = & end;
	}

	prev_heap_end = heap_end;
	heap_end += incr;

	return (void *) prev_heap_end;
}

int _getpid(void) { __not_implemented(); }
int _gettimeofday(struct timeval  *ptimeval, void *ptimezone) { __not_implemented(); }
int _kill(int32_t pid, int32_t sig)  { __not_implemented(); }
int _close(int32_t file) { __not_implemented(); }
int _fstat(int32_t file, struct stat *st) { __not_implemented(); }
int _isatty(int32_t file) { __not_implemented(); }
int _lseek(int32_t file, int32_t ptr, int32_t dir) { __not_implemented(); }
int _read(int32_t file, uint8_t *ptr, int32_t len) { __not_implemented(); }
int _readlink(const char *path, char *buf, size_t bufsize) { __not_implemented(); }
int _open(const uint8_t *path, int32_t flags, int32_t mode) { __not_implemented(); }
int _wait(int32_t *status) { __not_implemented(); }
int _unlink(const uint8_t *name) { __not_implemented(); }
int _times(struct tms *buf) { __not_implemented(); }
int _stat(const uint8_t *file, struct stat *st) { __not_implemented(); }
int _symlink(const char *path1, const char *path2)  { __not_implemented(); }
int _link(const uint8_t *old, const uint8_t *new) { __not_implemented(); }
int _fork(void) { __not_implemented(); }
int _execve(const uint8_t *name, uint8_t * const *argv, uint8_t * const *env) { __not_implemented(); }

