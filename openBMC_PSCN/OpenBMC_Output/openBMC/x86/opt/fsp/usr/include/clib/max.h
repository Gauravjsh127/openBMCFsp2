/*
 *   File: max.h
 * Author:
 *  Descr: typesafe max function
 *   Note:
 *   Date: 10/22/10
 */

#ifndef __MAX_H__
#define __MAX_H__

#define max(x,y)			\
({					\
    typeof(x) _x = (x);			\
    typeof(y) _y = (y);			\
    (void)((void*)&_x == (void*)&_y);	\
    _x < _y ? _y : _x;			\
})

#endif				/* __MAX_H__ */
