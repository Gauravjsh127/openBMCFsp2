/*
 *   File: timer.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Timer
 *   Note:
 *   Date: 10/03/10
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <sys/timerfd.h>

#include "attribute.h"
#include "builtin.h"
#include "assert.h"
#include "vector.h"

/* ==================================================================== */

typedef struct timer_struct timer_t;
typedef struct timer_callback_struct timer_callback_t;
typedef struct timer_event_struct timer_event_t;

typedef int (*timer_f) (timer_event *);

struct timer_struct {
	int fd;
	vector_t callbacks;
};

struct timer_callback_struct {
	void *data;
	timer_f func;
};

struct timer_event_struct {
};

/* ======================================================================= */

extern void timer_init(timer *, int) __nonnull((1));
extern void timer_delete(timer *) __nonnull((1));

extern int timer_fileno(timer *) __nonnull((1));

extern uint32_t timer_add(timer *, const char *, uint32_t,
			  timer_callback *) __nonnull((1, 2));
extern void timer_remove(timer *, uint32_t) __nonnull((1));

extern void timer_wait(timer *) __nonnull((1));

/* ======================================================================= */

#endif				/* __timer_H__ */
