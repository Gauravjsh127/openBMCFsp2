/*
 *   File: queue.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Queue container
 *   Note: This is an adaptor of the list container
 *   Date: 10/07/10
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "list.h"
#include "type.h"

typedef list queue;
typedef list_node queue_node;

#define queue_init(q)		list_init((list *)(q))
#define queue_push(q,n)		list_add_tail((list *)(q),(n))
#define queue_pop(q)		list_remove_head((list *)(q))
#define stack_empty(q)		list_empty((list *)(q))
#define queue_dump(q,s)		list_dump((list *)(q),(s))

#define queue_entry(n, t, m)	list_entry((n),(t),(m))
#define queue_top(q)		list_head((list *)(q))
#define queue_bottom(q)		list_tail((list *)(q))

#define queue_for_each(q, i, m)				\
    for (i = container_of_var(q->node.next, i, m);	\
        &i->m != &(q)->node;				\
        i = container_of_var(i->m.next, i, m))

#define queue_for_each_safe(q, i, n, m)			\
    for (i = container_of_var((q)->node.next, i, m),	\
            n = container_of_var(i->m.next, i, m);	\
        &i->m != &(q)->node;				\
        i = n, n = container_of_var(i->m.next, i, m))

#endif				/* __QUEUE_H__ */
