/*! @file array_iter.h
 *  @brief Sparse Array Iterator
 *  @details An array class whose elements are partitioned into data `pages'
 *           that are allocated upon first-touch.
 * @code
 * ...
 * array_t arr;
 * array_init(&arr, sizeof(uint32_t), 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &arr, VI_FLAG_FWD);
 *
 * uint32_t * j;
 * array_for_each(&it, j) {
 *     printf("arr[%d] = %d\n", it.idx, *j);
 * }
 * ...
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __ARRAY_ITER_H__
#define __ARRAY_ITER_H__

#include <stdbool.h>
#include <stdint.h>

#include "builtin.h"
#include "array.h"

/* ======================================================================= */

typedef struct array_iter array_iter_t;	//!< Alias for the @em array_iter class

/*!
 * @brief Array iterator
 * @details Array iterator class
*/
struct array_iter {
	array_t *array;		//!< Reference to the target array object
	int32_t idx;		//!< Current position of the iterator
	uint32_t flags;		//!< Iterator configuration flags
};

/* ======================================================================= */

#define AI_FLAG_NONE		0x00000000	//!< No flag mask
#define AI_FLAG_FWD		0x00000000	//!< Forward (FWD) flag mask
#define AI_FLAG_BWD		0x00000002	//!< Backward (BWD) flag mask
#define AI_FLAG_MASK		0x00000003	//!< All flags mask

/*!
 * @brief Initializes an @em array_iter iterator object
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &ar, AI_FLAG_FWD);
 * ...
 * @endcode
 * @memberof array_iter
 * @param self [in] array_iter object @em self pointer
 * @param array [in] array container object to iterate
 * @param flags [in] iterator configuration flags
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if @em array pointer is NULL
 */
extern void array_iter_init(array_iter_t * self, array_t * array,
			    uint32_t flags)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Resets an @em array iterator object
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &ar, AI_FLAG_FWD);
 * array_iter_clear(&it);
 * ...
 * @endcode
 * @memberof array_iter
 * @param self [in] array_iter object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void array_iter_clear(array_iter_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a pointer to @em array element bytes at the current iterator position
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &ar, AI_FLAG_FWD);
 * ...
 * data_t * d = (data_t *)array_iter_elem(&it);
 * ...
 * @endcode
 * @memberof array_iter
 * @param self [in] array_iter object @em self pointer
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern const void *array_iter_elem(array_iter_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn const void * array_iter_inc(array_iter_t * self, size_t count = 1)
 * @brief Increment the position of an @em array iterator
 * @details If the second (2nd) parameter is omitted, the iterator is incremented by one (1) position.
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &ar, AI_FLAG_FWD);
 * array_iter_inc(&it);
 * ...
 * @endcode
 * @memberof array_iter
 * @param self [in] array_iter object @em self pointer
 * @param count [in] Number of positions to increment (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define array_iter_inc(...) STRCAT(array_iter_inc, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const void *array_iter_inc1(array_iter_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const void *array_iter_inc2(array_iter_t * self, size_t count)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn const void * array_iter_dec(array_iter_t * self, size_t count = 1)
 * @brief decrement the position of an @em array iterator
 * @note If the second (2nd) parameter is omitted, the iterator is decremented by one (1) position.
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * array_iter_t it;
 * array_iter_init(&it, &ar, AI_FLAG_FWD);
 * array_iter_dec(&it, 3);
 * ...
 * @endcode
 * @memberof array_iter
 * @param self [in] array_iter object @em self pointer
 * @param count [in] Number of positions to decrement (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define array_iter_dec(...) STRCAT(array_iter_dec, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const void *array_iter_dec1(array_iter_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const void *array_iter_dec2(array_iter_t * self, size_t count)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @def array_for_each(it, i)
 * @hideinitializer
 * @brief Array for-each algorithm
 * @param it [in] Tree iterator object
 * @param i [in] Tree element variable
 */
#define array_for_each(it, i)				\
    for (i = (typeof(i))array_iter_elem(it);		\
         i != NULL;					\
         i = (typeof(i))array_iter_inc(it, 1))

/* ======================================================================= */

#endif				/* __ARRAY_ITER_H__ */
