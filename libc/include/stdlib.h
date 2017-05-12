#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL (void*)0x0

__attribute__((__noreturn__))
void abort(void);

#ifdef __cplusplus
}
#endif

#endif
