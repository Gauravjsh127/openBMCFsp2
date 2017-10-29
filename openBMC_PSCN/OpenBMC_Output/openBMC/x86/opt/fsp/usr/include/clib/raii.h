/*
 *   File: raii.h
 * Author:
 *  Descr: RAII macro
 *   Note:
 *   Date: 04/03/13
 */

#ifndef __RAII_H__
#define __RAII_H__

#include "attribute.h"

#define CLEANUP(type,name,func) 			\
    void __cleanup_ ## name (type * __v) { 		\
		if (__v != NULL)			\
			func(__v); 			\
    }							\
    type name __cleanup(__cleanup_##name)

#define RAII(type,name,ctor,dtor) 			\
    void __cleanup_##name (type * __v) { 		\
		if (__v != NULL && *__v != NULL) 	\
			dtor(*__v); 			\
    }							\
    type name __cleanup(__cleanup_##name) = (ctor)

#endif				/* __RAII_H__ */
