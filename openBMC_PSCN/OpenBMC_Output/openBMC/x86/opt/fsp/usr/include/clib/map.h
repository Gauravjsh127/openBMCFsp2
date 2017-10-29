/*
 *   File: map.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Map container
 *   Note:
 *   Date: 10/22/10
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>
#include <stdbool.h>

#include "builtin.h"
#include "compare.h"
#include "type.h"

#include "tree.h"

/* ==================================================================== */

typedef tree_t map_t;		//!< Alias for the @em map class
typedef tree_node_t map_node_t;	//!< Alias for the @em map_node class

/*!
 * @brief Constructs a @em map_node object
 * @memberof map_node
 * @param self [in] map_node object @em self pointer
 * @param key [in] pointer to key bytes
 * @return Reference to an initialized map_node object on SUCCESS
 */
static inline map_node_t *map_node_init(map_node_t *, const void *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @fn void map_init(map_t * self, compare_f cmp = default_compare)
 * @brief Constructs a @em map object
 * @memberof map
 * @param self [in] map_node object @em self pointer
 * @param cmp [in] Reference to the @em map_node compare function
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define map_init(...) STRCAT(map_init, NARGS(__VA_ARGS__))(__VA_ARGS__)
static inline int map_init1(map_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
static inline int map_init2(map_t *, compare_f)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @brief Insert a new @em map_node into the @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @param node [in] Reference to the @em map_node to insert
 * @return @em true if the @em map_node was inserted, @em false otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if @em map_node.key points to a duplicate key
 */
static inline int map_insert(map_t *, map_node_t *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Removes a @em map_node from the @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @param node [in] Reference to the @em map_node to remove
 * @return @em true if the @em map_node was removed, @em false otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline int map_remove(map_t *, map_node_t *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Find a @em map_node within the @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @param key [in] Reference to the @em key to find
 * @return Reference to a @em map_node on SUCCESS, false otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline map_node_t *map_find(map_t *, const void *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Hexdump the contents of a @em map to @em out output stream
 * @memberof map
 * @param self [in] map object @em self pointer
 * @param out [in] Reference to the @em out output stream
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
static inline void map_dump(map_t *, FILE *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Return whether a @em map container is empty
 * @memberof map
 * @param self [in] map object @em self pointer
 * @return @em true if @em map is empty, false otherwise
 */
static inline bool map_empty(map_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the node count of a @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @return @em 0 if @em map is empty, @em non-0 otherwise
 */
static inline size_t map_size(map_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the minimum @em map_node of a @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @return Reference to a @em map_node if @em self is non-NULL, NULL otherwise
 */
static inline map_node_t *map_min(map_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the maximum @em map_node of a @em map container
 * @memberof map
 * @param self [in] map object @em self pointer
 * @return Reference to a @em map_node if @em self is non-NULL, NULL otherwise
 */
static inline map_node_t *map_max(map_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ==================================================================== */

static inline map_node_t *map_node_init(map_node_t * self, const void *key)
{
	return (map_node_t *) tree_node_init((tree_node_t *) self, key);
}

/* ==================================================================== */

static inline int map_init1(map_t * self)
{
	assert(self != NULL);
	return tree_init((tree_t *) self, default_compare);
}

static inline int map_init2(map_t * self, compare_f cmp)
{
	assert(self != NULL);
	assert(cmp != NULL);
	return tree_init((tree_t *) self, cmp);
}

static inline int map_insert(map_t * self, map_node_t * node)
{
	assert(self != NULL);
	assert(node != NULL);
	return splay_insert((tree_t *) self, (tree_node_t *) node);
}

static inline int map_remove(map_t * self, map_node_t * node)
{
	assert(self != NULL);
	assert(node != NULL);
	return splay_remove((tree_t *) self, (tree_node_t *) node);
}

static inline map_node_t *map_find(map_t * self, const void *key)
{
	assert(self != NULL);
	assert(key != NULL);
	return (map_node_t *) tree_find((tree_t *) self, key);
}

static inline bool map_empty(map_t * self)
{
	assert(self != NULL);
	return tree_empty((tree_t *) self);
}

static inline void map_dump(map_t * self, FILE * out)
{
	assert(self != NULL);
	tree_dump((tree_t *) self, out ? out : stdout);
}

static inline size_t map_size(map_t * self)
{
	assert(self != NULL);
	return tree_size((tree_t *) self);
}

static inline map_node_t *map_min(map_t * self)
{
	assert(self != NULL);
	return tree_min((tree_t *) self);
}

static inline map_node_t *map_max(map_t * self)
{
	assert(self != NULL);
	return tree_max((tree_t *) self);
}

/* ==================================================================== */

#endif				/* __MAP_H__ */
