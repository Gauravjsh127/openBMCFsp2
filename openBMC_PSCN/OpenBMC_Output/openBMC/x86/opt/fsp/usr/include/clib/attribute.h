/*
 *   File: attribute.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: GCC attribute helpers
 *   Note:
 *   Date: 08/29/10
 */
/*! @file attribute.h
 *  @brief Attribute helpers
 *  @author Shaun Wetzstein <shaun@us.ibm.com>
 *  @date 2010-2011
 */

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

/*!
 * @def __constructor
 * @hideinitializer
 * @brief Constructor function attribute
 */
#define __constructor		__attribute__ ((constructor))

/*!
 * @def __destructor
 * @hideinitializer
 * @brief Desstructor function attribute
 */
#define __destructor		__attribute__ ((destructor))

/*!
 * @def __deprecated
 * @hideinitializer
 * @brief Deprecated function attribute
 */
#define __deprecated		__attribute__ ((deprecated))

/*!
 * @def __inline
 * @hideinitializer
 * @brief Inline function attribute
 */
#define __inline		__attribute__ ((always_inline))

/*!
 * @def __used
 * @hideinitializer
 * @brief Used identifier attribute
 */
#define __used			__attribute__ ((__used__))

/*!
 * @def __used
 * @hideinitializer
 * @brief Used identifier attribute
 */
#define __unused__		__attribute__ ((__unused__))

/*!
 * @def __const
 * @hideinitializer
 * @brief Const identifier attribute
 */
#define __const			__attribute__ ((__const__))

/*!
 * @def __must_check
 * @hideinitializer
 * @brief Warning about "unused" identifier attribute
 */
#define __must_check		__attribute__ ((warn_unused_result))

/*!
 * @def __packed
 * @hideinitializer
 * @brief Packed structure identifier attribute
 */
#define __packed		__attribute__ ((packed))

/*!
 * @def __cleanup
 * @hideinitializer
 * @brief Clenaup variable attribute
 */
#define __cleanup(f)		__attribute__ ((cleanup(f)))

#endif				/* __ATTRIBUTE_H__ */
