/*!
 * @file exception.h
 * @brief Exceptions for C
 * @details This file implements setjump / longjump based exceptions
 * @note Using these macros will create an exception context in each thread
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2008-2011
 */

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <setjmp.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define EX_PAGE_SIZE	4096	//!< Max. size of an exception payload
#define EXC_MAGIC	0x45584350	//!< Magic number, i.e. "EXCP"

#define ASSERTION	1	//!< Assertion exception class
//#define UNEXPECTED    2                       //!< Unexpected result exception class
//#define ERRNO         3                       //!< @em errno error exception class
#define EXC_LAST	4

typedef struct exception_frame exception_frame_t;	//!< Alias for the @em exception_frame class

/*!
 * @brief Exception class
 */
typedef struct {
	int ex;			//!< Exception class

	const char *file;	//!< Source file exception was thrown
	int line;		//!< Source line exception was thrown

	void *data;		//!< User-defined data associated with the exception
	int size;		//!< Size (in bytes) of the user-defined data
} exception_t;

/*!
 * @brief Exception frame class
 */
struct exception_frame {
	unsigned long magic;	//!< @private
	exception_frame_t *prev;	//!< @private
	jmp_buf jmp;		//!< @private
	exception_t exc;	//!< @private
};

/*!
 * @def try
 * @brief Simulate a try {...} catch {...} else {...} end_try block
 * @hideinitializer
 * @details For example,
 * @code
 * ...
 * #define ERROR 4
 * ...
 * exception_t * ex;
 * ...
 * try {
 *    ...
 * } catch (ERROR, ex) {
 *     fprintf(stderr, "caught ERROR exception at: %s(%d)\n",
 *             ex->file, ex->line);
 *     struct ErrorStruct * err = (struct ErrorStruct *)ex->data
 *     ...format and log err...
 * } end_try
 * @endcode
 */
#define try								\
do {									\
    __exc_init();							\
    exception_frame_t __frame;						\
    memset(&__frame, 0, sizeof(__frame));				\
    __frame.magic = EXC_MAGIC, __frame.prev = __exc_get_frame();	\
    __exc_set_frame(&__frame);						\
    volatile int __flag = setjmp(__frame.jmp);				\
    if (__flag == 0) {

/*!
 * @def catch
 * @brief Simulate a try {...} catch {...} else {...} end_try block
 * @hideinitializer
 * @details For example,
 * @code
 * ...
 * exception_t ex;
 * ...
 * try {
 *     ....
 * } catch (TYPE1, ex) {
 *     ...
 * } catch (TYPE2, ex) {
 *     ...
 * } catch (TYPE3, ex) {
 *     ...
 * } else (ex) {
 *     ...
 * } end_try
 * @endcode
 */
#define catch(x, e)							\
    } else if (__flag == (x)) {						\
        exception_frame_t * __tmp = __exc_get_frame();			\
        (e) = __tmp->exc, (e).ex = (__flag);				\

/*!
 * @def else
 * @brief Simulate a try {...} catch {...} else {...} end_try block
 * @hideinitializer
 * @details For example,
 * @code
 * ...
 * exception_t ex;
 * ...
 * try {
 *     ....
 * } catch (TYPE1, ex) {
 *     ...
 * } catch (TYPE2, ex) {
 *     ...
 * } catch (TYPE3, ex) {
 *     ...
 * } else (ex) {
 *     ...
 * } end_try
 * @endcode
 */
#define else(e) catch(__flag, (e))

/*!
 * @def end_try
 * @brief Simulate a try {...} catch {...} else {...} end_try block
 * @hideinitializer
 */
#define end_try								\
    } else {								\
        __exc_set_frame(__frame.prev);					\
        throw_bytes(__flag, __frame.exc.data, __frame.exc.size);	\
    }									\
    __exc_set_frame(__frame.prev);					\
} while (0);

/*!
 * @def throw(x, f, ...)
 * @brief Throw a C exception with printf-like formatting
 * @hideinitializer
 * @param x [in] Exception class
 * @param d [in] Pointer to user-defined data
 * @param s [in] Size of user-defined data (in bytes)
 * @details For example,
 * @code
 * ...
 * #define ERRNO_STRING
 * ...
 * #define throw_errno_string(x) \
 *         throw(ERRNO_STRING, "errno=%d : %s", (x),
 *               (void*)strerror((x)), __FILE__, __LINE__)
 * ...
 * if (rc < 0)
 *     throw_errno_string(errno);
 * ...
 * exception_t ex;
 * ...
 * catch (ERRNO_STRING ex) {
 *     fprintf(strerr, "EXCEPTION: errno: %s in file: %s on line: %d\n",
 *            (char *)ex.data, ex.file, ex.line);
 *     exit(1);
 * }
 * @endcode
 */
#define throw(x, f, ...)	({					\
    char __d[EX_PAGE_SIZE];						\
    __exc_throw((x), __d, snprintf(__d, sizeof __d, (f), ##__VA_ARGS__),\
                __FILE__, __LINE__);					\
})

/*!
 * @def rethrow(x, f, ...)
 * @brief Rethrow a C exception from within a try ... catch ... end_try block
 * @hideinitializer
 * @param e [in] Exception object
 * @details For example,
 * @code
 * ...
 * exception_t ex;
 * ...
 * catch (ERRNO_STRING ex) {
 *     fprintf(strerr, "EXCEPTION: errno: %s in file: %s on line: %d\n",
 *            (char *)ex.data, ex.file, ex.line);
 *     rethrow(ex);
 * }
 * @endcode
 */
#define rethrow(e)	({						\
    __exc_rethrow((e).ex, (e).data, (e).size, (e).file, (e).line);	\
})

/*!
 * @def throw_bytes(x, d, s)
 * @brief Throw a C exception with user-defined data
 * @hideinitializer
 * @param x [in] Exception class
 * @param d [in] Pointer to user-defined data
 * @param s [in] Size of user-defined data (in bytes)
 * @details For example,
 * @code
 * ...
 * #define ERRNO_STRING
 * ...
 * #define throw_errno_string(x) \
 *         throw_bytes(ERRNO_STRING, (void*)strerror((x)),
 *                     strlen(strerror((x)), __FILE__, __LINE__)
 * ...
 * if (rc < 0)
 *     throw_errno_string(errno);
 * ...
 * exception_t ex;
 * ...
 * catch (ERRNO_STRING ex) {
 *     fprintf(strerr, "EXCEPTION: errno: %s in file: %s on line: %d\n",
 *            (char *)ex.data, ex.file, ex.line);
 *     exit(1);
 * }
 * @endcode
 */
#define throw_bytes(x, d, s)						\
    __exc_throw((x), (d), (s), __FILE__, __LINE__)

/*!
 * @def throw_unexpected(x)
 * @brief Throw a message (NULL terminated C string), due to an @em
 * unexpected error
 * @hideinitializer
 * @param x [in] Unexpected error message
 * @details For example,
 * @code
 * ...
 * rc = foo_function();
 * if (rc < 0)
 *     throw_unexpected("Invalid return code from foo_function()");
 * ...
 * @endcode
 */
#define throw_unexpected(x) ({						\
    __exc_throw(UNEXPECTED,((void*)x), strlen((x)),			\
                __FILE__, __LINE__);					\
			     })

/*!
 * @def throw_errno(x)
 * @brief Throw an errno number, due to an invalid function result.
 * @hideinitializer
 * @param x [in] Unexpected errno number
 * @details For example,
 * @code
 * ...
 * rc = open(...);
 * if (rc == -1)
 *     throw_errno(errno);
 * ...
 * @endcode
 */
#define throw_errno(x) ({						\
    __exc_throw(ERRNO,(void*)(x), 0,					\
                __FILE__, __LINE__);					\
			})

/*! @cond */
extern void __exc_init(void);
extern exception_frame_t *__exc_get_frame(void);
extern void __exc_set_frame(exception_frame_t *);
extern int __exc_throw(int, void *, int, const char *, int);
extern int __exc_rethrow(int, void *, int, const char *, int);
extern void __exc_backtrace(const char *, ...);
extern const char *__exc_name(int exc);
/*! @endcond */

#endif				/* __EXCEPTION_H__ */
