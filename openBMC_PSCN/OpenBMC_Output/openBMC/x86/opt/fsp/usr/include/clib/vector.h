/*!
 * @file vector.h
 * @brief Vector container
 * @details Vectors are a container whose elements are in sequential order
 * @details For example,
 * @code
 * #include <clib/vector.h>
 * #include <clib/vector_iter.h>
 *
 * int main(const int argc, const char * argv[]) {
 *     vector_t a;
 *     vector_init(&a, 4, 1024);
 *
 *     vector_size(&a, 10);
 *
 *     int i;
 *     for (i=0; i<10; i++)
 *         vector_put(&a, i, &i);
 *     vector_put(&a, 223, (uint32_t[]){223});
 *
 *     vector_iter_t it;
 *     vector_iter_init(&it, &a, VI_FLAG_FWD);
 *
 *     uint32_t * j;
 *     vector_for_each(&it, j) {
 *         printf("vec[%d]\n", *j);
 *     }
 *
 *     vector_dump(&a, stdout);
 *     vector_delete(&a);
 *
 *     return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <sys/uio.h>

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#include "version.h"
#include "nargs.h"
#include "ident.h"
#include "mq.h"
#include "tree.h"

/* ======================================================================= */

#define VECTOR_MAGIC		"VCTR"

#define VECTOR_NAME_SIZE	40	//!< Maximum vector name size (in bytes)

#define VECTOR_FLAG_LSB		0x01	//!< Little-endian header data
#define VECTOR_FLAG_MSB		0x02	//!< Big-endian header data

#define VECTOR_ELEM_MIN		1	//!< Minimum element size (in bytes)
#define VECTOR_ELEM_MAX		8192	//!< Maximum element size (in bytes)

#define INIT_VECTOR_HEADER	{INIT_IDENT,{0,},0,0,0,0,0}
#define INIT_VECTOR		{INIT_VECTOR_HEADER, INIT_TREE}

/*!
 * @brief vector container header
 */
struct vector_header {
	ident_t id;		//!< identification
	char name[VECTOR_NAME_SIZE];	//!< vector name

	size_t page_size;	//!< data page size (in bytes)
	size_t page_count;	//!< number of data pages allocated (currently)

	size_t elem_size;	//!< element size (in bytes)
	size_t elem_num;	//!< element count (per page)

	size_t size;		//!< number of initialized elements
};
typedef struct vector_header vector_header_t;	//!< Alias for the @em vector_header class

/*!
 * @brief vector container
 */
struct vector {			//! The vector class
	vector_header_t hdr;	//!< Table metadata

	tree_t tree;		//!< @private
};
typedef struct vector vector_t;	//!< Alias for the @em vector class

/* ======================================================================= */

/*!
 * @fn void vector_init(vector_t * self, const char * name, size_t elem_size [, size_t page_size])
 * @brief Constructs an @em vector container object
 * @details For example,
 * @code
 * ...
 * vector_t ar;
 * vector_init(&ar, 4, 1024);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param name [in] vector object @em name string
 * @param elem_size [in] vector element size, in bytes
 * @param page_size [in] size of page, in bytes
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_init(...) STRCAT(vector_init, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern int vector_init3(vector_t *, const char *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern int vector_init4(vector_t *, const char *, size_t, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @brief Destructs an @em vector container object
 * @details Deallocate all backing storage associated with this \em vector object
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 524, &count);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return None
 */
extern int vector_delete(vector_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Returns a reference to the element at position @em idx in the @em vector
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 524, &count);
 * printf("ar[524] = %d\n", *(int *)vector_at(&ar, 524));
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param idx [in] vector element index
 * @return Reference to vector element at @em idx on SUCCESS
 * @throws UNEXPECTED if @em self pointer is NULL
 * @throws UNEXPECTED if vector element at @em idx is uninitialized
 */
extern const void *vector_at(vector_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn void vector_get(vector_t * self, size_t idx, const void * ptr, size_t count=1)
 * @brief Copy content from the @em vector
 * @details Copies @em elem_num element(s) starting at position @em elem_off in the source @em vector to destination pointer @em ptr
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 524, &count);
 * vector_get(&ar, 524, &count);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param idx [in] vector element index
 * @param ptr [out] Destination storage pointer
 * @param count [in] Desgination element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_get(...) STRCAT(vector_get, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern int vector_get3(vector_t *, size_t, void *)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern int vector_get4(vector_t *, size_t, void *, size_t)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn void vector_put(vector_t * self, size_t idx, const void * ptr, size_t count=1)
 * @brief Assign new content to the @em vector
 * @details Copies @em elem_num element(s) from source pointer @em ptr to the destination @em vector starting at position @em elem_off
 * @note If the fourth parameter is omitted, it defaults to 1
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 524, &count);
 * vector_get(&ar, 524, &count);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param idx [in] vector element index
 * @param ptr [in] Source storage pointer
 * @param count [in] Source element count (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_put(...) STRCAT(vector_put, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern int vector_put3(vector_t *, size_t, const void *)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
extern int vector_put4(vector_t *, size_t, const void *, size_t)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @fn size_t vector_size(vector_t * self, size_t size = 1)
 * @brief Return or set the size of the @em vector
 * @details Return or set the number of allocated elements in the @em vector
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_size(&ar, 2040);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param size [in] New vector size
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
/*! @cond */
#define vector_size(...) STRCAT(vector_size, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern size_t vector_size1(vector_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern int vector_size2(vector_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
/*! @endcond */

/*!
 * @brief Return pages of the @em vector container
 * @details Return the number of pages in the @em vector container
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_size(&ar, 2040);
 * printf("pages = %d\n", vector_pages(&ar));
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return The number of pages that conform the vector's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t vector_pages(vector_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return capacity of the @em vector
 * @details Return the number of allocated and unallocated elements in the @em vector container
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_size(&ar, 2040);
 * printf("capacity = %d\n", vector_capacity(&ar));
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return The number of total elements that conform the vector's content
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern size_t vector_capacity(vector_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern size_t vector_elem_size(vector_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Save (write) an @em vector object to a stream
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 7, &count);
 * vector_put(&ar, 7000, &count);
 * ...
 * FILE * f = fopen("...", "w");
 * ...
 * vector_save(&ar, f);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param out [in] save destination stream
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern ssize_t vector_save(vector_t *, FILE *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Load (read) a @em vector object from a stream
 * @details For example,
 * @code
 * ...
 * vector ar;
 * ...
 * FILE * f = fopen("...", "r");
 * ...
 * vector_load(&ar, f);
 * vector_dump(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @param in [in] load source stream
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern ssize_t vector_load(vector_t *, FILE *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Send (write) an @em vector object to a message queue
 * @details For example,
 * @code
 * ...
 * vector_init(&ar, 4, 1024);
 * vector_put(&ar, 7, &count);
 * vector_put(&ar, 7000, &count);
 * ...
 * mqueue mq;
 * mqueue_init(&mq, "my_server");
 * mqueue_create(&mq, gettid());
 * ...
 * vector_send(&ar, &mq);
 * vector_delete(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int vector_send(vector_t *, mqueue_t *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief receive (read) an @em vector object from a message queue
 * @details For example,
 * @code
 * ...
 * vector ar;
 * ...
 * mqueue mq;
 * mqueue_open(&mq, path);
 * ...
 * vector_receive(&ar, &mq);
 * vector_dump(&ar);
 * ...
 * @endcode
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return non-0 on success
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int vector_receive(vector_t *, mqueue_t *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Pretty print the contents of an @em vector to stdout
 * @memberof vector
 * @param self [in] vector object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void vector_dump(vector_t *, FILE *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ======================================================================= */

#endif				/* __VECTOR_H__ */
