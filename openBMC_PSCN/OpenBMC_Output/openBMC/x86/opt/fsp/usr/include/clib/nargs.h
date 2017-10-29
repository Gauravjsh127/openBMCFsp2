/*!
 * @file nargs.h
 * @brief Macro utilities
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __NARGS_H__
#define __NARGS_H__

/*!
 * @def STRCAT(x,y)
 * @hideinitializer
 * @brief C string concatination of @em x and @em y
 * @param x [in] C-style string
 * @param y [in] C-style string
 */
/*! @cond */
#define STRCAT(x,y)	__C__(x, y)
#define __C__(x,y)	x ## y
/*! @endcond */

/*!
 * @def NARGS(...)
 * @hideinitializer
 * @brief Return the number of pre-process macro parameters
 */
/*! @cond */
#define __NARGS__(junk, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _, ...) _
#define NARGS(...) __NARGS__(junk, ##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/*! @endcond */

#endif				/* __NARGS_H__ */
