/*
 *   File: watch.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Directory activity callback monitor
 *   Note:
 *   Date: 10/03/10
 */

#ifndef __WATCH_H__
#define __WATCH_H__

#include <stdint.h>
#include <stdbool.h>

#include <sys/inotify.h>

#include "attribute.h"
#include "builtin.h"
#include "assert.h"

#include "array.h"

/* ==================================================================== */

#define WATCH_ACCESS		IN_ACCESS
#define WATCH_ATTRIB		IN_ATTRIB
#define WATCH_CLOSE_WRITE	IN_CLOSE_WRITE
#define WATCH_CLOSE_NOWRITE	IN_CLOSE_NOWRITE
#define WATCH_CREATE		IN_CREATE
#define WATCH_DELETE		IN_DELETE
#define WATCH_DELETE_SELF	IN_DELETE_SELF
#define WATCH_MODIFY		IN_MODIFY
#define WATCH_MOVE_SELF		IN_MOVE_SELF
#define WATCH_MOVE_FROM		IN_MOVE_FROM
#define WATCH_MOVE_TO		IN_MOVE_TO
#define WATCH_OPEN		IN_OPEN

typedef struct watch watch_t;
typedef struct watch_callback watch_callback_t;
typedef struct inotify_event watch_event_t;

typedef int (*watch_f) (watch_event_t *);

struct watch {
	int fd;
	array_t callbacks;
};

struct watch_callback {
	void *data;
	watch_f func;
};

/* ======================================================================= */

extern void watch_init(watch_t * self) __nonnull((1));
extern void watch_delete(watch_t * self) __nonnull((1));

extern int watch_fileno(watch_t * self) __nonnull((1));

extern uint32_t watch_add(watch_t * self, const char *path, uint32_t events,
			  watch_callback_t * cb) __nonnull((1, 2));
extern void watch_remove(watch_t * self, uint32_t wd) __nonnull((1));

extern void watch_wait(watch_t * self) __nonnull((1));

/* ======================================================================= */

#endif				/* __watch_H__ */
