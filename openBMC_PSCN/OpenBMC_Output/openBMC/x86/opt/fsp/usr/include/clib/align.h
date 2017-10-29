/*!
 * @file align.h
 * @brief Alignment helpers
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __ALIGN_H__
#define __ALIGN_H__

/*!
 * @def alignof(t)
 * @hideinitializer
 * @brief Returns the alignment of an object or minimum alignment required by a type
 * @param t [in] Object or type name
 */
#define alignof(t)			__alignof__(t)

#ifndef offsetof

/*!
 * @def offsetof(t,m)
 * @hideinitializer
 * @brief Returns the offset of a member within a structure
 * @param t [in] Structure type name
 * @param m [in] Member name within a structure
 */
#define offsetof(t,m)			__builtin_offsetof(t, m)
#endif

#endif				/* __ALIGN_H__ */
