/*
 *   File: heap.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Arena based (slab) memory allocator
 *   Note:
 *   Date: 07/28/10
 */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>

#include "slab.h"

/* ======================================================================= */

typedef struct heap heap_t;

struct heap {
	size_t page_size;
	size_t alloc_size;

	size_t slab_size;
	slab_t *slab[];
};

/* ======================================================================= */

#define heap_init(a,c) (__heap_init((h),(a),(c),__FILE__,__LINE__))
extern void __heap_init(heap_t * self, size_t alloc_size, size_t cache_size,
			const char *file, int line);

#define heap_new(a,c) (__heap_new((a),(c),__FILE__,__LINE__))
extern heap_t *__heap_new(size_t alloc_size, size_t cache_size,
			  const char *file, int line);
#define heap_delete(h) (__heap_delete((h),__FILE__,__LINE__))
extern void __heap_delete(heap_t * self, const char *file,
			  int line) __nonnull((1));

#define heap_alloc(h,s) (__heap_alloc((h),(s),__FILE__,__LINE__))
extern void *__heap_alloc(heap_t * self, size_t size, const char *file,
			  int line) __nonnull((1));
#define heap_free(h,p) (__heap_free((h),(p),__FILE__,__LINE__))
extern void __heap_free(heap_t * self, void *ptr, const char *file,
			int line) __nonnull((1, 2));

extern void heap_dump(heap_t * self, FILE * out) __nonnull((1));

/* ======================================================================= */

#endif				/* __HEAP_H__ */
