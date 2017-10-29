/*
 *   File: signal.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: File descriptor activity callback signaler
 *   Note:
 *   Date: 10/03/10
 */

#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <stdint.h>
#include <stdbool.h>

#include <sys/epoll.h>
#include <sys/signalfd.h>

#include "attribute.h"
#include "builtin.h"
#include "assert.h"

#include "tree.h"
#include "array.h"

/* ======================================================================= */

typedef struct signal signal_t;
typedef struct signalfd_siginfo signal_event_t;

struct signal {
	int fd;

	sigset_t mask;
	int flags;
};

/* ======================================================================= */

extern void signal_init(signal_t * self) __nonnull((1));
extern void signal_delete(signal_t * self) __nonnull((1));

extern int signal_fileno(signal_t * self) __nonnull((1));

extern int signal_setmask(signal_t * self, const sigset_t mask) __nonnull((1));

#if 0
#define signalfd_wait(...) STRCAT(signalfd_wait, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void signal_wait1(signal_t *) __nonnull((1));
extern void signal_wait2(_sisignal_tgnal *, int) __nonnull((1));
#endif

/* ======================================================================= */

#endif				/* __SIGNAL_H__ */
