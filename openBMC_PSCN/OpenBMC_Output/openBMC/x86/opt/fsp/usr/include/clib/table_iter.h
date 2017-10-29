/*!
 * @file table_iter.h
 * @brief Table Iterator
 * @code
 * ...
 * table_iter_t it;
 * table_iter_init(&it, &t);
 *
 * value_t * val;
 * table_for_each(&it, val) {
 *     if (value_type(val) != VT_UNKNOWN)
 *         value_dump(val, stdout);
 * }
 * ...
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __TABLE_ITER_H__
#define __TABLE_ITER_H__

#include <stdint.h>
#include <stdbool.h>

#include "attribute.h"

#include "value.h"
#include "table.h"
#include "vector_iter.h"

/* ======================================================================= */

typedef struct table_iter table_iter_t;	//!< Alias for the @em table class

/*!
 * @brief Table iterator
 */
struct table_iter {
	table_t *table;		//!< Reference to the target table object
	vector_iter_t it;	//!< Current position of the table
	uint32_t flags;		//!< Iterator configuration flags
};

/* ==================================================================== */

#define TI_FLAG_NONE		0x00000000	//!< No flag mask
#define TI_FLAG_FWD		0x00000001	//!< Forward (FWD) flag mask
#define TI_FLAG_BWD		0x00000002	//!< Backwards (BWD) flag mask
#define TI_FLAG_MASK		0x00000003	//!< All flag mask

/*!
 * @brief Initializes an @em table_iter iterator object
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @param table [in] table container object to iterate
 * @param flags [in] iterator configuration flags
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if @em table pointer is NULL
 */
extern int table_iter_init(table_iter_t *, table_t *, uint32_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Resets an @em table iterator object
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int table_iter_clear(table_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a pointer to @em value element at the current iterator position
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern const value_t *table_iter_elem(table_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Increment the position of an @em table iterator
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @param count [in] Number of positions to increment
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
#define table_iter_inc(...) STRCAT(table_iter_inc, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const value_t *table_iter_inc1(table_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const value_t *table_iter_inc2(table_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Decrement the position of an @em table iterator
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @param count [in] Number of positions to decrement
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
#define table_iter_dec(...) STRCAT(table_iter_dec, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const value_t *table_iter_dec1(table_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern const value_t *table_iter_dec2(table_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn const void * table_iter_pos(table_iter_t * self, size_t pos)
 * @brief Return or set the iterator position witin the @em table
 * @details If the second (2nd) parameter is omitted, this functions returns
 * the current position.
 * @memberof table_iter
 * @param self [in] table_iter object @em self pointer
 * @param pos [in] new iterator position (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define table_iter_pos(...) STRCAT(table_iter_pos, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern size_t table_iter_pos1(table_iter_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern int table_iter_pos2(table_iter_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @def table_for_each_row(it, i)
 * @hideinitializer
 * @brief Table for-each-row element algorithm
 * @param it [in] Table iterator object
 * @param r [in] Value element variable (row)
 */
#define table_for_each(it,r)					\
    for (r = (typeof(r))table_iter_elem((it));			\
         r != NULL;						\
         r = (typeof(r))table_iter_inc((it)))

/* ==================================================================== */

#endif				/* __TABLE_ITER_H__ */
