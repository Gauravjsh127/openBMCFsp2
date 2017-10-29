/*! @file vector_iter.h
 *  @brief Vector Iterator
 *  @details Vectors are a kind of sequence container. As such, their elements
 *           are ordered following a strict linear sequence.
 * @code
 * ...
 * vector_t vec;
 * vector_init(&vec, sizeof(uint32_t), 1024);
 * ...
 * vector_iter_t it;
 * vector_iter_init(&it, &vec, VI_FLAG_FWD);
 *
 * uint32_t * j;
 * vector_for_each(&it, j) {
 *     printf("vec[%d] = %d\n", it.idx, *j);
 * }
 * ...
 * @endcode
 *  @author Shaun Wetzstein <shaun@us.ibm.com>
 *  @date 2010-2011
 */

#ifndef __VECTOR_ITER_H__
#define __VECTOR_ITER_H__

#include <stdbool.h>
#include <stdint.h>

#include "builtin.h"
#include "vector.h"

/* ======================================================================= */

typedef struct vector_iter vector_iter_t;	//!< Alias for the @em vector_iter class

/*!
 * @brief vector iterator
 * @details Vector iterator class
 */
struct vector_iter {
	vector_t *vector;	//!< Reference to the target vector object
	size_t idx;		//!< Current position of the iterator
	uint32_t flags;		//!< Iterator configuration flags
};

/* ======================================================================= */

#define VI_FLAG_NONE		0x00000000	//!< No flag mask
#define VI_FLAG_FWD		0x00000000	//!< Forward (FWD) flag mask
#define VI_FLAG_BWD		0x00000002	//!< Backward (BWD) flag mask
#define VI_FLAG_MASK		0x00000003	//!< All flags mask

/*!
 * @brief Initializes an @em vector_iter iterator object
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @param vector [in] vector container object to iterate
 * @param flags [in] iterator configuration flags
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if @em array pointer is NULL
 */
extern int vector_iter_init(vector_iter_t *, vector_t *, uint32_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Resets an @em vector iterator object
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int vector_iter_clear(vector_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a pointer to @em vector element bytes at the current iterator position
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern const void *vector_iter_elem(vector_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn const void * vector_iter_inc(vector_iter_t * self, size_t count)
 * @brief Increment the position of an @em vector iterator
 * @details If the second (2nd) parameter is omitted, the iterator is
 * incremented by one (1) position.
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @param count [in] Number of positions to increment (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_iter_inc(...) STRCAT(vector_iter_inc, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const void *vector_iter_inc1(vector_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const void *vector_iter_inc2(vector_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn const void * vector_iter_dec(vector_iter_t * self, size_t count)
 * @brief decrement the position of an @em vector iterator
 * @note If the second (2nd) parameter is omitted, the iterator is decremented by one (1) position.
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @param count [in] Number of positions to decrement (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_iter_dec(...) STRCAT(vector_iter_dec, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const void *vector_iter_dec1(vector_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const void *vector_iter_dec2(vector_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn const void * vector_iter_pos(vector_iter_t * self, size_t pos)
 * @brief Return or set the iterator position witin the @em vector
 * @details If the second (2nd) parameter is omitted, this functions returns the current position.
 * @memberof vector_iter
 * @param self [in] vector_iter object @em self pointer
 * @param pos [in] new iterator position (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_iter_pos(...) STRCAT(vector_iter_pos, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern size_t vector_iter_pos1(vector_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern int vector_iter_pos2(vector_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @def vector_for_each(it, i)
 * @hideinitializer
 * @brief Vector for-each algorithm
 * @param it [in] Tree iterator object
 * @param i [in] Tree element variable
 */
#define vector_for_each(it, i)				\
    for (i = (typeof(i))vector_iter_elem(it);		\
         i != NULL;					\
         i = (typeof(i))vector_iter_inc(it))

/* ======================================================================= */

#endif				/* __VECTOR_ITER_H__ */
