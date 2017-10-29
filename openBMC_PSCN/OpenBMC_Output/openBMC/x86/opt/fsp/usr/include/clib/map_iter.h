/*
 *   File: map_iter.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Iterator for the Map container
 *   Note:
 *   Date: 10/22/10
 */

#ifndef __MAP_ITER_H__
#define __MAP_ITER_H__

#include <stdint.h>
#include <stdbool.h>

#include "builtin.h"

#include "map.h"
#include "tree_iter.h"

/* ==================================================================== */

typedef tree_iter_t map_iter_t;

/* ==================================================================== */

#define MI_FLAG_NONE		TI_FLAG_NONE
#define MI_FLAG_FWD		TI_FLAG_FWD
#define MI_FLAG_BWD		TI_FLAG_BWD

/*!
 * @brief Initializes an @em map_iter iterator object
 * @memberof map_iter
 * @param self [in] map_iter object @em self pointer
 * @param map [in] map container object to iterate
 * @param flags [in] iterator configuration flags
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if @em map pointer is NULL
 */
static inline int map_iter_init(map_iter_t *, map_t *, uint32_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Resets an @em map iterator object
 * @memberof map_iter
 * @param self [in] map_iter object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline int map_iter_clear(map_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a pointer to a @em map_node element at the current iterator
 * position
 * @memberof map_iter
 * @param self [in] map_iter object @em self pointer
 * @return non-NULL on success, NULL otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline map_node_t *map_iter_elem(map_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Increment the position of an @em map iterator
 * @memberof map_iter
 * @param self [in] map_iter object @em self pointer
 * @param count [in] Number of positions to increment
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline map_node_t *map_iter_inc(map_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Decrement the position of an @em map iterator
 * @memberof map_iter
 * @param self [in] map_iter object @em self pointer
 * @param count [in] Number of positions to decrement
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline map_node_t *map_iter_dec(map_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @def map_for_each(it, i, m)
 * @hideinitializer
 * @brief Tree for-each algorithm
 * @param it [in] Tree iterator object
 * @param i [in] Tree element variable
 * @param m [in] Member name
 */
#define map_for_each(it, i, m)						    \
    for (map_iter_clear(it), i = container_of_var(map_iter_elem(it), i, m); \
         map_iter_elem(it) != NULL;					    \
         i = container_of_var(map_iter_inc(it), i, m))

/* ==================================================================== */

static inline int map_iter_init(map_iter_t * self, map_t * map, uint32_t flags)
{
	assert(self != NULL);
	assert(map != NULL);
	return tree_iter_init((tree_iter_t *) self, (tree_t *) map, flags);
}

int map_iter_clear(map_iter_t * self)
{
	assert(self != NULL);
	return tree_iter_clear((tree_iter_t *) self);
}

static inline map_node_t *map_iter_elem(map_iter_t * self)
{
	assert(self != NULL);
	return (map_node_t *) tree_iter_elem((tree_iter_t *) self);
}

static inline map_node_t *map_iter_inc(map_iter_t * self)
{
	assert(self != NULL);
	return (map_node_t *) tree_iter_inc((tree_iter_t *) self);
}

static inline map_node_t *map_iter_dec(map_iter_t * self)
{
	assert(self != NULL);
	return (map_node_t *) tree_iter_dec((tree_iter_t *) self);
}

/* ==================================================================== */

#endif				/* __MAP_ITER_H__ */
