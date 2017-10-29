/*!
 * @file bitset.h
 * @brief bitset container
 * @details bitsets are a special container that are design to store
 *          bits (elements with only 2 possible values: 0 or 1)
 * @details For example,
 * @code
 * #include <clib/bitset.h>
 * #include <clib/bitset_iter.h>
 *
 * int main(const int argc, const char * argv[]) {
 *     bitset_t a;
 *     bitset_init(&a, 4, 1024);
 *
 *     bitset_size(&a, 10);
 *
 *     int i;
 *     for (i=0; i<10; i++)
 *         bitset_put(&a, i, &i);
 *     bitset_put(&a, 223, (uint32_t[]){223});
 *
 *     bitset_iter_t it;
 *     bitset_iter_init(&it, &a, VI_FLAG_FWD);
 *
 *     uint32_t * j;
 *     bitset_for_each(&it, j) {
 *         printf("vec[%d]\n", *j);
 *     }
 *
 *     bitset_dump(&a, stdout);
 *     bitset_delete(&a);
 *
 *     return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __bitset_H__
#define __bitset_H__

#include <stdbool.h>
#include <stdint.h>

#include "builtin.h"

#include "nargs.h"
#include "mqueue.h"
#include "tree.h"

/* ======================================================================= */

typedef struct bitset bitset_t;	//!< Alias for the @em bitset class

/*!
 * @brief bitset container
 * @details bitset container
 */
struct bitset {			//! The bitset class
	uint32_t magic;		//!< bitset magic number

	uint32_t page_size;	//!< bitset data page size (in bytes)

	uint16_t elem_size;	//!< bitset element size (in bytes)
	uint16_t elem_num;	//!< bitset element count (per page)

	size_t size;		//!< Number of initialized elements
	size_t pages;		//!< Number of data pages allocated (currently)

	tree_t tree;		//!< @private
};

/* ======================================================================= */

/*!
 * @brief Constructs an @em bitset container object
 * @details For example,
 * @code
 * ...
 * bitset_t ar;
 * bitset_init(&ar, 4, 1024);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @param elem_size [in] bitset element size, in bytes
 * @param elem_num [in] bitset element number, per data page
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define bitset_init(...) STRCAT(bitset_init, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void bitset_init2(bitset_t * self, size_t elem_size)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern void bitset_init3(bitset_t * self, size_t elem_size, size_t page_size)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @brief Destructs an @em bitset container object
 * @details Deallocate all backing storage associated with this \em bitset object
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_put(&ar, 524, &count);
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return None
 */
extern void bitset_delete(bitset_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Returns a reference to the element at position @em idx in the @em bitset
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_put(&ar, 524, &count);
 * printf("ar[524] = %d\n", *(int *)bitset_at(&ar, 524));
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @param elem_num [in] bitset element index
 * @return Reference to bitset element at @em idx on SUCCESS
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if bitset element at @em idx is uninitialized
 */
extern const void *bitset_at(bitset_t * self, size_t elem_num)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn void bitset_get(bitset_t * self, size_t elem_off, const void * ptr, size_t elem_num=1)
 * @brief Copy content from the @em bitset
 * @details Copies @em elem_num element(s) starting at position @em elem_off in the source @em bitset to destination pointer @em ptr
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_put(&ar, 524, &count);
 * bitset_get(&ar, 524, &count);
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @param elem_off [in] bitset element index
 * @param ptr [out] Destination storage pointer
 * @param elem_num [in] Desgination element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define bitset_get(...) STRCAT(bitset_get, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void bitset_get3(bitset_t * self, size_t elem_off, void *ptr)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern void bitset_get4(bitset_t * self, size_t elem_off, void *ptr,
			size_t elem_num)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn void bitset_put(bitset_t * self, size_t elem_off, const void * ptr, size_t elem_num=1)
 * @brief Assign new content to the @em bitset
 * @details Copies @em elem_num element(s) from source pointer @em ptr to the destination @em bitset starting at position @em elem_off
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_put(&ar, 524, &count);
 * bitset_get(&ar, 524, &count);
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @param elem_off [in] bitset element index
 * @param ptr [in] Source storage pointer
 * @param elem_num [in] Source element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define bitset_put(...) STRCAT(bitset_put, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern void bitset_put3(bitset_t * self, size_t elem_off, const void *ptr)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern void bitset_put4(bitset_t * self, size_t elem_off, const void *ptr,
			size_t elem_num)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn size_t bitset_size(bitset_t * self, size_t size = 1)
 * @brief Return or set the size of the @em bitset
 * @details Return or set the number of allocated elements in the @em bitset
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_size(&ar, 2040);
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @param size [in] New bitset size
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define bitset_size(...) STRCAT(bitset_size, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern size_t bitset_size1(bitset_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern void bitset_size2(bitset_t * self, size_t size)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @brief Return pages of the @em bitset container
 * @details Return the number of pages in the @em bitset container
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_size(&ar, 2040);
 * printf("pages = %d\n", bitset_pages(&ar));
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return The number of pages that conform the bitset's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t bitset_pages(bitset_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return capacity of the @em bitset
 * @details Return the number of allocated and unallocated elements in the @em bitset container
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_size(&ar, 2040);
 * printf("capacity = %d\n", bitset_capacity(&ar));
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return The number of total elements that conform the bitset's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t bitset_capacity(bitset_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Send (write) an @em bitset object to a message queue
 * @details For example,
 * @code
 * ...
 * bitset_init(&ar, 4, 1024);
 * bitset_put(&ar, 7, &count);
 * bitset_put(&ar, 7000, &count);
 * ...
 * mqueue mq;
 * mqueue_init(&mq, "my_server");
 * mqueue_create(&mq, gettid());
 * ...
 * bitset_send(&ar, &mq);
 * bitset_delete(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void bitset_send(bitset_t * self, mqueue_t * mq)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief receive (read) an @em bitset object from a message queue
 * @details For example,
 * @code
 * ...
 * bitset ar;
 * ...
 * mqueue mq;
 * mqueue_open(&mq, path);
 * ...
 * bitset_receive(&ar, &mq);
 * bitset_dump(&ar);
 * ...
 * @endcode
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void bitset_receive(bitset_t * self, mqueue_t * mq)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Pretty print the contents of an @em bitset to stdout
 * @memberof bitset
 * @param self [in] bitset object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void bitset_dump(bitset_t * self, FILE * out)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ======================================================================= */

#endif				/* __bitset_H__ */
