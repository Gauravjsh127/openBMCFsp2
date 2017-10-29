/*!
 * @file slab.h
 * @brief Slab allocator
 * @details A slab allocator is a bit-mapped allocator for which all allocations are the same size
 * @details For example,
 * @code
 * #include <clib/slab.h>
 *
 * #define SIZE 20000
 *
 * int main(const int argc, const char * argv[]) {
 *     slab_t s = INIT_SLAB;
 *     slab_init(&s, "my_slab", 16, 4096);
 *
 *     void * ptr[SIZE] = {NULL,};
 *
 *     int i;
 *     for (i=0; i<SIZE; i++) {
 *         ptr[i] = slab_alloc(&s);
 *     }
 *
 *     for (i=0; i<SIZE; i++) {
 *         slab_free(&s, ptr[i]);
 *     }
 *
 *     slab_delete(&s);
 *
 *     return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __SLAB_H__
#define __SLAB_H__

#include <stdbool.h>
#include <stdint.h>

#include "ident.h"
#include "nargs.h"
#include "tree.h"

/* ======================================================================= */

#define SLAB_MAGIC		"SLAB"

#define SLAB_NAME_SIZE		40	//!< Maximum vector name size (in bytes)

#define SLAB_FLAG_LSB		0x01	//!< Little-endian header data
#define SLAB_FLAG_MSB		0x02	//!< Big-endian header data

#define SLAB_ALLOC_MIN		4
#define SLAB_ALLOC_MAX		8192

#define INIT_SLAB_HEADER	{INIT_IDENT,{0,},0,0,0,0,0,0}
#define INIT_SLAB		{INIT_SLAB_HEADER,INIT_TREE}

/*!
 * @brief Slab allocator header
 */
struct slab_header {
	ident_t id;		//!< identification
	char name[SLAB_NAME_SIZE];	//!< slab name

	size_t page_size;	//!< page size
	size_t align_size;	//!< page alignment size
	size_t page_count;	//!< page count

	size_t alloc_size;	//!< allocation size
	size_t data_size;	//!< data size
	size_t bitmap_size;	//!< bitmap size
};
typedef struct slab_header slab_header_t;

/*!
 * @brief Slab allocator
 */
struct slab {			//!< The slab class
	slab_header_t hdr;

	tree_t tree;		//!< @private
};
typedef struct slab slab_t;	//!< Alias for the @em slab class

/* ======================================================================= */

/*!
 * @brief Constructs a @em slab allocator
 * @memberof slab
 * @param self [in] self object
 * @name name [in] name strnig ('\0' terminated)
 * @param alloc_size [in] allocation size (in bytes)
 * @param page_size [in] page size (in bytes)
 * @return None
 */
/*! @cond */
#define slab_init(...) STRCAT(slab_init, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern int slab_init3(slab_t * self, const char *name, size_t alloc_size)
__nonnull((1, 2));
extern int slab_init4(slab_t * self, const char *name, size_t alloc_size,
		      size_t page_size) __nonnull((1, 2));
extern int slab_init5(slab_t * self, const char *name, size_t alloc_size,
		      size_t page_size, size_t align_size) __nonnull((1, 2));
/*! @endcond */

/*!
 * @brief Deallocate the memory associated with a @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @note Upon success, the cache tree is deallocated from the heap
 * @return None
 */
extern int slab_delete(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Allocate first available slot in the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-NULL on success, NULL otherwise
 */
extern void *slab_alloc(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Deallocate a slot in the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @param ptr [in] pointer to a slab allocation
 * @note Upon success, a slab slot is deallocated from the slab object
 * @return None
 */
extern int slab_free(slab_t * self, void *ptr)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Return the allocation (i.e. slot) size of the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-0 on success, 0 otherwise
 */
extern size_t slab_alloc_size(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the page (i.e. slot) size of the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-0 on success, 0 otherwise
 */
extern size_t slab_page_size(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the data size of the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-0 on success, 0 otherwise
 */
extern size_t slab_data_size(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the map (i.e. slot) size of the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-0 on success, 0 otherwise
 */
extern size_t slab_bitmap_size(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the alignment size of the @em slab allocator
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return non-0 on success, 0 otherwise
 */
extern size_t slab_align_size(slab_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Pretty print the contents of a @em slab to stdout
 * @memberof slab
 * @param self [in] slab object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern void slab_dump(slab_t * self, FILE * out)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ======================================================================= */

#endif				/* __SLAB_H__ */
