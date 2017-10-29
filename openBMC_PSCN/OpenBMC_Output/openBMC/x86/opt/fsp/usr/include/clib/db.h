/*!
 * @file db.h
 * @brief Database client
 * @details Embedded database API wrapper
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __DB_H__
#define __DB_H__

#include <stdbool.h>
#include <stdint.h>
#include <sqlite3.h>

#include "exception.h"

/* ======================================================================= */

#define DB_ROW			SQLITE_ROW	//!< Statement contains another row @hideinitializer
#define DB_DONE			SQLITE_DONE	//!< Statement contains no more rows @hideinitializer
#define DB_OK			SQLITE_OK	//!< Database command ok @hideinitializer

#define DB			4	//!< Database exception class
#define SQL			5	//!< Statement exception class

typedef struct db db_t;		//!< Alias for the @em db class
typedef struct statement statement_t;	//!< Alias for the @em statement class

/*!
 * @brief Database access class
 */
struct db {
#ifdef SQLITE3
	const char *path;	//!< @private
	sqlite3 *db;		//!< @private
#endif
};

typedef enum transaction_type transaction_type_t;	//!< Alias for the @em transaction_type enum

/*!
 * @brief Transaction types
 */
enum transaction_type {
	tt_ERROR = -1,		//!< Invalid / unknown transaction
	tt_DEFERRED,		//!< Defferred locking transaction
	tt_IMMEDIATE,		//!< Immediate locking transaction
	tt_EXCLUSIVE,		//!< Exclusive locking transaction
};

#ifdef SQLITE3
typedef sqlite3_context db_context_t;	//!< User-defined database context
typedef sqlite3_value db_value_t;	//!< User-defined value
#else
#error MUST define db_context_t and db_value_t
#endif

/*!
 * @brief User-defined function
 */
typedef void (*db_f) (db_context_t *, int, db_value_t **);

/*!
 * @brief Statement class
 */
struct statement {
	db_t *db;		//!< Database object
#ifdef SQLITE3
	sqlite3_stmt *stmt;	//!< @private
#endif
};

/* ======================================================================= */

#if 0
extern db_t *db_new(const char *) __nonnull((1));
#endif

/*!
 * @brief Constructs a @em db object
 * @memberof db
 * @param self [in] db object @em self pointer
 * @param path [in] db file name
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_init(db_t * self, const char *path)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Destructs a @em db object
 * @memberof db
 * @param self [in] db object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_delete(db_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Execute a SQL txt statement
 * @memberof db
 * @param self [in] db object @em self pointer
 * @param fmt [in] printf-like format string
 * @param ... [in] printf-like arguments
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_execute(db_t * self, const char *fmt, ...)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Open a @em db object
 * @memberof db
 * @param self [in] db object @em self pointer
 * @param flags [in] Open flagsprintf-like format string
 * @return 0 on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_open(db_t * self, int flags)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Close a @em db object
 * @memberof db
 * @param self [in] db object @em self pointer
 * @return 0 on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_close(db_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Register a user-defined function on a @em db object
 * @memberof db
 * @param self [in] db object @em self pointer
 * @param name [in] db function name
 * @param argc [in] Function argument count
 * @param func [in] User-defined function
 * @param data [in] User-defined data passed to @em finc
 * @return 0 on siccess, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_register_function(db_t * self, const char *name, int argc,
				db_f func, void *data)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Start a database transaction
 * @memberof db
 * @param self [in] db object @em self pointer
 * @param type [in] Transaction type
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_begin(db_t * self, transaction_type_t type)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Commit a database transaction
 * @memberof db
 * @param self [in] db object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_commit(db_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Rollback a database transaction
 * @memberof db
 * @param self [in] db object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int db_rollback(db_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/* ======================================================================= */

#if 0
extern statement_t *statement_new(db_t *) __nonnull((1));
#endif

/*!
 * @brief Constructs a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param db [in] Pointer to owning database object
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_init(statement_t * self, db_t * db)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Destructs a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_delete(statement_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Prepare a @em statement object for execution
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @return DB_ROW when a row is available, DB_DONE when no more rows left
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_prepare(statement_t * self, const char *sql)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Execute the next step of @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_step(statement_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Reset a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_reset(statement_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Finalize a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_finalize(statement_t * self)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Binds an integer to a bound variable of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Bound varialble position number
 * @param val [in] Integer value
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_bind_int(statement_t * self, int pos, int val)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Binds a long long integer to a bound variable of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Bound varialble position number
 * @param val [in] Long Long Integer value
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_bind_int64(statement_t * self, int pos, int64_t val)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Binds a text string to a bound variable of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Bound varialble position number
 * @param val [in] Text string value
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_bind_text(statement_t * self, int pos, const char *val)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;

/*!
 * @brief Binds a blob to a bound variable of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Bound varialble position number
 * @param val [in] Blob value
 * @param len [in] Lengh of blob value (in bytes)
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_bind_blob(statement_t * self, int pos, const void *val,
			       int len)
/*! @cond */
__nonnull((1, 3)) /*! @endcond */ ;

/*!
 * @brief Return an integer from the result set of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Column position number
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_column_int(statement_t * self, int pos)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return an long long integer from the result set of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Column position number
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int64_t statement_column_int64(statement_t * self, int pos)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return the length of a column from the result set of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Column position number
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int statement_column_bytes(statement_t * self, int pos)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a text string from the result set of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Column position number
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern const unsigned char *statement_column_text(statement_t * self, int pos)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Return a blob from the result set of a @em statement object
 * @memberof statement
 * @param self [in] statement object @em self pointer
 * @param pos [in] Column position number
 * @return DB_OK on success, non-0 otherwise
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern const void *statement_column_blob(statement_t * self, int pos)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

#ifdef SQLITE3
#define DBERR(d)			({				\
  err_t * _e = (err_t  *)malloc(sizeof(*_e) + ERR_DATA_SIZE);		\
  memset(_e, 0, sizeof(*_e) + ERR_DATA_SIZE);				\
  _e->size = snprintf(_e->data, ERR_DATA_SIZE, "%s (code=%d)",		\
		      sqlite3_errmsg(d), sqlite3_errcode(d));		\
  _e->magic = ERR_MAGIC, _e->type = DB, _e->code = sqlite3_errcode(d);	\
  _e->file = __FILE__, _e->line = __LINE__; 				\
  err_put(_e);								\
					})

#define SQLERR(d,s)			({				\
  err_t * _e = (err_t  *)malloc(sizeof(*_e) + ERR_DATA_SIZE);		\
  memset(_e, 0, sizeof(*_e) + ERR_DATA_SIZE);				\
  _e->size = snprintf(_e->data, ERR_DATA_SIZE, "'%s' => %s (code=%d)",	\
		      sqlite3_sql(s), sqlite3_errmsg(d),		\
		      sqlite3_errcode(d));				\
  _e->magic = ERR_MAGIC, _e->type = SQL, _e->code = sqlite3_errcode(d);	\
  _e->file = __FILE__, _e->line = __LINE__; 				\
  err_put(_e);								\
					})
#if 0
#define throw_db(x) ({							\
            char __e[strlen(sqlite3_errmsg((x))) + 32];			\
            __exc_throw(DB,((void*)__e),				\
                        sprintf(__e, "(%d) %s",				\
                                sqlite3_errcode((x)),			\
                                sqlite3_errmsg((x))),			\
                        __FILE__, __LINE__);				\
		     })

#define throw_statement(x,s) ({						\
            char __e[strlen(sqlite3_errmsg((x))) +			\
                     strlen(sqlite3_sql((s))) + 32];			\
            __exc_throw(STATEMENT,((void*)__e),				\
                        sprintf(__e, "(%d) %s\n%s",			\
                               sqlite3_errcode((x)),			\
                               sqlite3_errmsg((x)),			\
                               sqlite3_sql((s))),			\
                        __FILE__, __LINE__);				\
			      })
#endif
#else
#error FIX ME
#endif

/* ======================================================================= */

#endif				/* __DB_H__ */
