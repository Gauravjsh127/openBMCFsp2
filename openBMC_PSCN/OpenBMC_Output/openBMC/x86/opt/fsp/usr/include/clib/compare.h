/*!
 * @file compare.h
 * @brief Generic compare function
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __COMPARE_H__
#define __COMPARE_H__

/*!
 * @brief Pointer to a generic compare function
 */
typedef int (*compare_f) (const void *, const void *);

/*!
 * @brief Default compare function
 */
static inline int default_compare(const void *v1, const void *v2)
{
	const int i1 = (const int)v1, i2 = (const int)v2;
	return i1 - i2;
}

#endif				/* __COMPARE_H__ */
