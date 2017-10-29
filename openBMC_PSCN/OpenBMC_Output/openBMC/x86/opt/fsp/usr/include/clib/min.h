/*
 *   File: min.h
 * Author:
 *  Descr: typesafe min function
 *   Note:
 *   Date: 10/22/10
 */

#ifndef __MIN_H__
#define __MIN_H__

#define min(x,y)			\
({					\
    typeof(x) _x = (x);			\
    typeof(y) _y = (y);			\
    (void)((void*)&_x == (void*)&_y);	\
    _x < _y ? _x : _y;			\
})

#endif				/* __MIN_H__ */
