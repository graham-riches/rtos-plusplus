/**
*****************************************************************************
**
**  File        : syscalls.c
**
**  Author		: Auto-generated by System workbench for STM32
**
**  Abstract    : System Workbench Minimal System calls file
**
** 		          For more information about which c-functions
**                need which of these lowlevel functions
**                please consult the Newlib libc-manual
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**   1. Redistributions of source code must retain the above copyright notice,
**      this list of conditions and the following disclaimer.
**   2. Redistributions in binary form must reproduce the above copyright notice,
**      this list of conditions and the following disclaimer in the documentation
**      and/or other materials provided with the distribution.
**   3. Neither the name of STMicroelectronics nor the names of its contributors
**      may be used to endorse or promote products derived from this software
**      without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*****************************************************************************
*/

/* Includes */
#include "common.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>


/* Variables */
//#undef errno
extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;


/* Functions */
void initialise_monitor_handles()
{
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
    PARAMETER_NOT_USED( pid );
    PARAMETER_NOT_USED( sig );
	errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}		/* Make sure we hang here */
}

__attribute__((weak)) int _read(int file, char *ptr, int len)
{
    PARAMETER_NOT_USED( file );
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		*ptr++ = (char)__io_getchar();
	}

return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
    PARAMETER_NOT_USED( file );
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr)
	{
//		write(1, "Heap and stack collision\n", 25);
//		abort();
		errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int _close(int file)
{
    PARAMETER_NOT_USED( file );
	return -1;
}


int _fstat(int file, struct stat *st)
{
    PARAMETER_NOT_USED( file );
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
    PARAMETER_NOT_USED( file );
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
    PARAMETER_NOT_USED( file );
    PARAMETER_NOT_USED( ptr );
    PARAMETER_NOT_USED( dir );
	return 0;
}

int _open(char *path, int flags, ...)
{
    PARAMETER_NOT_USED( path );
    PARAMETER_NOT_USED( flags );
	/* Pretend like we always fail */
	return -1;
}

int _wait(int *status)
{
    PARAMETER_NOT_USED( status );
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
    PARAMETER_NOT_USED( name );
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf)
{
    PARAMETER_NOT_USED( buf );
	return -1;
}

int _stat(char *file, struct stat *st)
{
    PARAMETER_NOT_USED( file );
    PARAMETER_NOT_USED( st );
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
    PARAMETER_NOT_USED( old );
    PARAMETER_NOT_USED( new );
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
    PARAMETER_NOT_USED( name );
    PARAMETER_NOT_USED( argv );
    PARAMETER_NOT_USED( env );
	errno = ENOMEM;
	return -1;
}
