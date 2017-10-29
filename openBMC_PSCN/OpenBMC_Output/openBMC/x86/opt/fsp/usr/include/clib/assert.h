/*! @file assert.h
 *  @brief Assertion helpers
 *  @author Shaun Wetzstein <shaun@us.ibm.com>
 *  @date 2010-2011
 */

#ifdef assert
#undef assert
#endif

/*!
 * @def assert(e)
 * @hideinitializer
 * @brief Check for program assertion failures
 * @param e [in] assertion expression
 * @throws ASSERTION iff expression @em e evaluates @em false
 */

#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "exception.h"
#define assert(e) ((void)((e) ? (void)0 : throw_bytes(ASSERTION, __STRING((e)), strlen(__STRING((e))))))
#endif
