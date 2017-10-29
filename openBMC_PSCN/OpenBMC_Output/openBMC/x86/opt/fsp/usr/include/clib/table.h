/*!
 * @file table.h
 * @brief Table Container
 * @details For example,
 * @code
 * #include <clib/array.h>
 * #include <clib/array_iter.h>
 *
 * int main(const int argc, const char * argv[]) {
 *     table t;
 *     table_init(&t, COLS);
 *
 *     const char * str = "column0 is a really long string";
 *
 *     table_name(&t, "table name");
 *     table_name(&t, 0, str);
 *     table_name(&t, 1, "column1");
 *     table_name(&t, 2, "column2");
 *     table_name(&t, 3, "column3");
 *     table_name(&t, 4, "column4");
 *
 *     printf("%s\n", table_name(&t, 0));
 *
 *     size_t r, c;
 *     value v;
 *
 *     for (r=0; r<COLS; r++) {
 *         for (c=0; c<COLS; c++) {
 *             table_put(&t, r, c,
 *                       value_set(&v, r * COLS + c));
 *
 *             printf("t[%d][%d] --> %d\n", r, c, r * COLS + c);
 *         }
 *     }
 *
 *     table_iter it;
 *     table_iter_init(&it, &t);
 *
 *     value * val;
 *     table_for_each(&it, val)
 *         value_dump(val, stdout);
 *
 *     table_delete(&t);
 *
 *     return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "compare.h"
#include "vector.h"
#include "value.h"
#include "mqueue.h"

/* ======================================================================= */

#define TABLE_NAME_SIZE		52	//!< Maximum table name size (in bytes)

#define TABLE_MAGIC		"TBLE"	//!< Table magic number

#define TABLE_FLAG_LSB		0x01	//!< little-endian header data
#define TABLE_FLAG_MSB		0x02	//!< big-endian header data

#define INIT_TABLE_HEADER	{INIT_IDENT,{0,},0}
#define INIT_TABLE		{INIT_TABLE_HEADER,INIT_VECTOR,INIT_VECTOR,INIT_VECTOR}

/*!
 * @brief table container header
 */
struct table_header {
	ident_t id;		//!< Identification
	char name[TABLE_NAME_SIZE];	//!< Table name

	size_t col_nr;		//!< Number of columns
};
typedef struct table_header table_header_t;	//!< Alias for the @em table_header class

/*!
 * @brief table container
 */
struct table {			//!< The Table class
	table_header_t hdr;	//!< Table metadata

	vector_t table;		//!< @private
	vector_t string;	//!< @private
	vector_t blob;		//!< @private
};
typedef struct table table_t;	//!< Alias for the @em table class

/* ==================================================================== */

/*!
 * @brief Constructs a @em table object
 * @memberof table
 * @param self [in] table object @em self pointer
 * @param name [in] table object @em name string
 * @param col_nr [in] Number of columns
 * @return None
 */
extern int table_init(table_t *, const char *, size_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Destructs an @em table container object
 * @details Deallocate all backing storage associated with this \em table object
 * @memberof table
 * @param self [in] table object @em self pointer
 * @return None
 */
extern int table_delete(table_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Copy content from the @em table
 * @memberof table
 * @param self [in] table object @em self pointer
 * @param row [in] Row number
 * @param col [out] Column number
 * @return Reference to @em value on success, NULL otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern value_t *table_get(table_t *, size_t, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @fn const void * table_row(table_iter_t * self, size_t row_nr,
 * [ value_t * row ])
 */
/*! @cond */
#define table_row(...) STRCAT(table_row, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern value_t *table_row2(table_t *, size_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;
extern int table_row3(table_t *, size_t, value_t *)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;
/* !endcond */

extern value_t *table_column(table_t *, value_t *, size_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Assign new content to the @em table
 * @memberof table
 * @param self [in] table object @em self pointer
 * @param row [in] Row number
 * @param col [in] Column number
 * @param value [in] Reference to source @em value
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int table_put(table_t *, size_t, size_t, value_t *)
/*! @cond */
__nonnull((1, 4)) /*! @endcond */ ;

/*!
 * @fn const char * table_name(table_t * self, size_t col, const char * name,
 * size_t len)
 * @brief Set or return the name of a column within a @em table
 * @note If the fourth parameter is omitted, the @em name is assumed to contain
 * a trailing NULL-byte
 * @note If the third parameter is omitted, the column's current name is
 * returned
 * @memberof table
 * @param self [in] table object @em self pointer
 * @param col [in] Column number
 * @param name [in] New column name string (optional)
 * @param len [in] New column name string length (optional)
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */

/*! @cond */
#define table_name(...) STRCAT(table_name, NARGS(__VA_ARGS__))(__VA_ARGS__)
extern const char *table_name2(table_t *, size_t) __nonnull((1));

#define table_name3(t,c,s)						\
    choose_expr(const_expr(s),						\
                _table_name3((t),(c),(const char *)(s)),		\
                _table_name3((t),(c),(const char *)(s)))

extern int _table_name3(table_t *, size_t, const char *) __nonnull((1, 3));

#define table_name4(t,c,s,l)						\
    choose_expr(const_expr(s),						\
                _table_name4((t),(c),(const char *)(s),(l)),		\
                _table_name4((t),(c),(const char *)(s),(l)))

extern int _table_name4(table_t *, size_t, const char *, size_t)
__nonnull((1, 3));
/*! @endcond */

/*!
 * @brief Return the number of rows in the @em table
 * @memberof table
 * @param self [in] table object @em self pointer
 * @return None
 */
extern size_t table_rows(table_t *) /*! @cond */ __nonnull((1))	/*! @endcond */
;

/*!
 * @brief Return the number of column in the @em table
 * @memberof table
 * @param self [in] table object @em self pointer
 * @return None
 */
extern size_t table_columns(table_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern int table_serialize(table_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern int table_deserialize(table_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern ssize_t table_save(table_t *, FILE *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

extern ssize_t table_load(table_t *, FILE *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

extern void table_print(table_t *, FILE *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern void table_dump(table_t *, FILE *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

extern int table_sort(table_t *, compare_f)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/* ==================================================================== */

#endif				/* __TABLE_H__ */
