/*!
 * @file array.h
 * @brief Array container
 * @details An array class whose elements are partitioned into data `pages'
 *          that are allocated upon first-touch, possibly leading to a pages
 *          that are sparsely populated.
 * @details For example,
 * @code
 * #include <clib/array.h>
 * #include <clib/array_iter.h>
 *
 * int main(const int argc, const char * argv[]) {
 *     array_t a;
 *     array_init(&a, 4, 1024);
 *
 *     array_dump(&a, stdout);
 *
 *     array_put(&a, 52, (uint32_t[]){52});
 *     array_put(&a, 53, (uint32_t[]){53});
 *     array_put(&a, 167, (uint32_t[]){167});
 *     array_put(&a, 223, (uint32_t[]){223});
 *     array_put(&a, 78, (uint32_t[]){78});
 *     array_put(&a, 205, (uint32_t[]){205});
 *     array_put(&a, 183, (uint32_t[]){183});
 *     array_put(&a, 150, (uint32_t[]){150});
 *     array_put(&a, 90, (uint32_t[]){90});
 *     array_put(&a, 66, (uint32_t[]){66});
 *     array_put(&a, 91, (uint32_t[]){91});
 *     array_put(&a, 45, (uint32_t[]){45});
 *     array_put(&a, 211, (uint32_t[]){211});
 *
 *     uint32_t arr[] = {985,986,987,988,990,991,992,993,994};
 *     array_put(&a, 985, arr, 9);
 *
 *     array_iter_t it;
 *     array_iter_init(&it, &a, AI_FLAG_FWD);
 *
 *     uint32_t * j;
 *     array_for_each(&it, j) {
 *         printf("arr[%d]\n", *j);
 *     }
 *
 *     array_dump(&a, stdout);
 *     array_delete(&a);
 *
 *     return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdbool.h>
#include <stdint.h>

#include "builtin.h"
#include "mqueue.h"
#include "tree.h"
#include "nargs.h"

/* ======================================================================= */

typedef struct array array_t;	//!< Alias for the @em array class

/*!
 * @brief Array container class
 */
struct array {
	uint32_t magic;		//!< Array magic number

	uint32_t page_size;	//!< Array data page size (in bytes)

	uint16_t elem_size;	//!< Array element size (in bytes)
	uint16_t elem_num;	//!< Array element count (per page)

	size_t size;		//!< Number of initialized elements
	size_t pages;		//!< Number of data pages allocated (currently)

	size_t low;		//!< @private
	size_t high;		//!< @private

	size_t map_size;	//!< @private
	tree_t tree;		//!< @private
};

/* ======================================================================= */

/*!
 * @brief Constructs an @em array container object
 * @details For example,
 * @code
 * ...
 * array_t ar;
 * array_init(&ar, 4, 1024);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param elem_size [in] array element size, in bytes
 * @param elem_num [in] array element number, per data page
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void array_init(array_t * self, size_t elem_size, size_t elem_num)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Destructs an @em array container object
 * @details Deallocate all backing storage associated with this \em array object
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 524, &count);
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return None
 */
extern void array_delete(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Returns a reference to the element at position @em idx in the @em array
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 524, &count);
 * printf("ar[524] = %d\n", *(int *)array_at(&ar, 524));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param elem_num [in] array element index
 * @return Reference to array element at @em idx on SUCCESS
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if array element at @em idx is uninitialized
 */
extern const void *array_at(array_t * self, size_t elem_num)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn void array_get(array_t * self, size_t elem_off, const void * ptr, size_t elem_num=1)
 * @brief Copy content from the @em array
 * @details Copies @em elem_num element(s) starting at position @em elem_off in the source @em array to destination pointer @em ptr
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 524, &count);
 * array_get(&ar, 524, &count);
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param elem_off [in] array element index
 * @param ptr [out] Destination storage pointer
 * @param elem_num [in] Desgination element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define array_put(...) STRCAT(array_put, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void array_get3(array_t * self, size_t elem_off, void *ptr)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern void array_get4(array_t * self, size_t elem_off, void *ptr,
		       size_t elem_num)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn void array_put(array_t * self, size_t elem_off, const void * ptr, size_t elem_num=1)
 * @brief Assign new content to the @em array
 * @details Copies @em elem_num element(s) from source pointer @em ptr to the destination @em array starting at position @em elem_off
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 524, &count);
 * array_get(&ar, 524, &count);
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param elem_off [in] array element index
 * @param ptr [in] Source storage pointer
 * @param elem_num [in] Source element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define array_put(...) STRCAT(array_put, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void array_put3(array_t * self, size_t elem_off, const void *ptr)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern void array_put4(array_t * self, size_t elem_off, const void *ptr,
		       size_t elem_num)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn bool array_status(array_t * self, size_t elem_num, bool state)
 * @brief Set or return the initialized status of the @em array element at position @em idx
 * @note If the third parameter is omitted, the element's current status is returned
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 524, &count);
 * printf("%d %d\n", array_status(&ar, 524), array_status(&ar, 600));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param elem_num [in] array element index
 * @param state [in] Element state (optional)
 * @return None
 * @throws UNEXPECTED if @em idx is out of bounds
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define array_status(...) STRCAT(array_status, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern bool array_status2(array_t * self, size_t elem_num)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern bool array_status3(array_t * self, size_t elem_num, bool state)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn size_t array_size(array_t * self, size_t size = 1)
 * @brief Return or set the size of the @em array
 * @details Return or set the number of allocated elements in the @em array
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_size(&ar, 2040);
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @param size [in] New array size
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
#define array_size(...) STRCAT(array_size, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern size_t array_size1(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern void array_size2(array_t * self, size_t size)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return pages of the @em array container
 * @details Return the number of pages in the @em array container
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_size(&ar, 2040);
 * printf("pages = %d\n", array_pages(&ar));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return The number of pages that conform the array's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t array_pages(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return capacity of the @em array
 * @details Return the number of allocated and unallocated elements in the @em array container
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_size(&ar, 2040);
 * printf("capacity = %d\n", array_capacity(&ar));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return The number of total elements that conform the array's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t array_capacity(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return @em array index lower bound
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 7, &count);
 * array_put(&ar, 7000, &count);
 * printf("low = %d\n", array_low(&ar));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t array_low(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return @em array index upper bound
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 7, &count);
 * array_put(&ar, 7000, &count);
 * printf("high = %d\n", array_high(&ar));
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t array_high(array_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Send (write) an @em array object to a message queue
 * @details For example,
 * @code
 * ...
 * array_init(&ar, 4, 1024);
 * array_put(&ar, 7, &count);
 * array_put(&ar, 7000, &count);
 * ...
 * mqueue mq;
 * mqueue_init(&mq, "my_server");
 * mqueue_create(&mq, gettid());
 * ...
 * array_send(&ar, &mq);
 * array_delete(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void array_send(array_t * self, mqueue_t * mq)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief receive (read) an @em array object from a message queue
 * @details For example,
 * @code
 * ...
 * array ar;
 * ...
 * mqueue mq;
 * mqueue_open(&mq, path);
 * ...
 * array_receive(&ar, &mq);
 * array_dump(&ar);
 * ...
 * @endcode
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void array_receive(array_t * self, mqueue_t * mq)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Pretty print the contents of an @em array to stdout
 * @memberof array
 * @param self [in] array object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void array_dump(array_t * self, FILE * out)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ======================================================================= */

#endif				/* __ARRAY_H__ */
