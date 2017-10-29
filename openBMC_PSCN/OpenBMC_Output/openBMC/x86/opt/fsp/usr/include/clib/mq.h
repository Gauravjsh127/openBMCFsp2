/*!
 * @file mqueue.h
 * @brief Message queues
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __MQUEUE_H__
#define __MQUEUE_H__

#include <stdint.h>
#include <stdbool.h>
#include <mqueue.h>

#include "attribute.h"

/* ==================================================================== */

typedef struct mqueue mqueue_t;	//!< Alias for the @em mqueue class
typedef struct mq_attr mqueue_attr_t;

#define	INIT_MQUEUE	{NULL,-1,-1}

/*!
 * @brief POSIX message queues
 */
struct mqueue {
	char *service;		//!< Message queue name

	mqd_t in;		//!< Inbound message queue
	mqd_t out;		//!< Outbound message queue
};

/* ==================================================================== */

/*!
 * @brief Constructs a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param service [in] mqueue service name
 * @return None
 * @throws UNEXPECTED if @em self pointer is NULL
 */
extern int mqueue_init(mqueue_t *, const char *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Destructs a @em mqueue object
 * @details Closes all message queues associated with this \em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @return None
 */
extern int mqueue_delete(mqueue_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Create the server-side of a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param tid [in] thread id
 * @return None
 */
extern int mqueue_create(mqueue_t *, pid_t)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Create the client-side of a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param path [in] FIX ME
 * @return None
 */
extern int mqueue_open(mqueue_t *, char *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Close a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param path [in] FIX ME
 * @return None
 */
extern int mqueue_close(mqueue_t *, char *)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Get message queue operating attributes of a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @return Initialized mqueue_attr object
 */
extern mqueue_attr_t mqueue_getattr(mqueue_t *)
/*! @cond */
__nonnull((1)) /*! @endcond */ ;

/*!
 * @brief Write (send) a message to a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param ptr [in] Reference to data bytes to send
 * @param ptr [in] Length (in bytes) of the data bytes
 * @return non-0 on success, 0 othereise
 */
extern int mqueue_send(mqueue_t *, void *, size_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

/*!
 * @brief Read (receive) a message from a @em mqueue object
 * @memberof mqueue
 * @param self [in] mqueue object @em self pointer
 * @param ptr [out] Reference to data bytes to send
 * @param ptr [in] Length (in bytes) of the buffer referenced by @em ptr
 * @return non-0 on success, 0 othereise
 */
extern int mqueue_receive(mqueue_t *, void *, size_t)
/*! @cond */
__nonnull((1, 2)) /*! @endcond */ ;

#endif				/* __MQUEUE_H__ */
