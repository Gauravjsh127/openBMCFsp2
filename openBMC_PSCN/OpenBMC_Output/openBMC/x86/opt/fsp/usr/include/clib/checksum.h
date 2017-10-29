/*! @file checksum.h
 *  @brief Simple XOR checksum
 *  @author Mike Kobler <mkobler@us.ibm.com>
 *  @date 2007-2012
 */

#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#include <stdint.h>

/*!
 * @brief Copy bytes from the source reference to the destination reference
 *        while computing a 32-bit checksum
 * @param __dst [in] Destination reference
 * @param __src [in] Source reference (must be 4 byte aligned)
 * @param __n [in] Number of bytes to copy / compute
 * @return 32-bit Checksum value
 */
extern uint32_t memcpy_checksum(void *__restrict __dst,
				const void *__restrict __src, size_t __n)
/*! @cond */
__THROW __nonnull((2)) /*! @endcond */ ;

#endif				/* __CHECKSUM_H__ */
