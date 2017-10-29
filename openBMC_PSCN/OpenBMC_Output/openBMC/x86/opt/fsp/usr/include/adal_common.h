 /*****************************************************************************
 *
 * IBM Confidential
 *
 * Licensed Internal Code Source Materials
 *
 * IBM Flexible Support Processor Licensed Internal Code
 *
 * (c) Copyright IBM Corp. 2004
 *
 * The source code is for this program is not published or otherwise divested
 * of its trade secrets, irrespective of what has been deposited with the
 * U.S. Copyright Office.
 *
 *****************************************************************************/

#ifndef _ADAL_COMMON_H
#define _ADAL_COMMON_H

#include <stdint.h>
#include <inttypes.h>
//#include <sys/types.h>
#include <ctype.h>
#include "trace_adal.h"

#define TYPE_UINT64	1
#define TYPE_FLOAT	2
#define TYPE_STRING	3

#define DEFAULT_FMT_SZ		256
extern char g_time_format[DEFAULT_FMT_SZ];

/*
 * @brief Time stamp contains seconds and microseconds
 */
#define TRACE_TIME_REAL   0
/*
 * @brief Time stamp contains a 64 bit counter from a 50MHZ cpu
 */
#define TRACE_TIME_50MHZ  1
/*
 * @brief Time stamp contains a 64 bit counter from a 167MHZ cpu (1000/6)
 */
#define TRACE_TIME_167MHZ 3
/*
 * @brief Time stamp contains a 64 bit counter from a 200MHZ cpu
 */
#define TRACE_TIME_200MHZ 2
#define TRACE_TIME_TIMESPEC 4
/*
 * @brief Time stamp is "Epoch" based (0 equals 1970-01-01 00:00:00 UTC)
 */
#define TRACE_TIME_EPOCH 128

#define TRACE_TIME_UNKNOWN 255

#define TRACE_TIME_MAX 3

/*
 * @brief maximum value of arguments to trace_adal_write_all.
 */
#define MAX_ARGS 9

/* a trace entry with args
 * the args can be accessed via h.args[..] too
 */
struct trace_write_entry {
    trace_entry_head_t h;
    uint32_t args[9];
};
typedef struct trace_write_entry trace_write_entry_t;

/* struct for SET_DEBUG/CONSOLE/PIPE ioctl calls */
struct trace_set_config {
	union {
		const char *name;
		trace_desc_t td;
	} u;
	int newlevel;
};

/* trace descriptor with console level
 */
struct trace_set_console {
    trace_desc_t td;
    uint32_t level;
};
typedef struct trace_set_console trace_set_console_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief change case to upper
 */
static inline void toupper_string(char *text)
{
	size_t j;

	for (j = 0; j < strlen(text); j++) {
		text[j] = toupper(text[j]);
	}
}


/*
 * @brief print to stdout dump of src
 */
void print_dump(const char *src, int len);

#ifdef __cplusplus
}
#endif

// verbose types
#define TRACE_TYPE_ERROR		1	//  error descriptions
#define TRACE_TYPE_PERROR		2	//  error descriptions with perror call
#define TRACE_TYPE_INFO		4	//  runtime info
#define TRACE_TYPE_VARS		8	//  interpreted data output
#define TRACE_TYPE_FUNC		16	//  functions entry/leave
#define TRACE_TYPE_RAW		32	//  raw data output
#define TRACE_TYPE_DEBUG		64	//  other debug output

// verbose levels (-v parameter)
#define TRACE_LVL0		0
#define TRACE_LVL1		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR;
#define TRACE_LVL2		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR | TRACE_TYPE_INFO;
#define TRACE_LVL3		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR | TRACE_TYPE_INFO | TRACE_TYPE_VARS;
#define TRACE_LVL4		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR | TRACE_TYPE_INFO | TRACE_TYPE_VARS | TRACE_TYPE_FUNC;
#define TRACE_LVL5		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR | TRACE_TYPE_INFO | TRACE_TYPE_VARS | TRACE_TYPE_FUNC | TRACE_TYPE_RAW;
#define TRACE_LVL6		TRACE_TYPE_ERROR | TRACE_TYPE_PERROR | TRACE_TYPE_INFO | TRACE_TYPE_VARS | TRACE_TYPE_FUNC | TRACE_TYPE_RAW | TRACE_TYPE_DEBUG;

#define TRACE_LVL_ALL	TRACE_LVL6;

// verbose macroses
#define TRACE( TYPE, LVL, FMT, args...) \
	if( TYPE & LVL ) \
	{\
		if( TYPE & (TRACE_TYPE_INFO | TRACE_TYPE_VARS | TRACE_TYPE_FUNC | TRACE_TYPE_RAW | TRACE_TYPE_DEBUG) ) \
			printf("%s %s: " FMT, __FILE__, __func__ , ## args);\
		\
		else if( TYPE & TRACE_TYPE_DEBUG ) \
			printf("D: %s %s: " FMT, __FILE__, __func__ , ## args);\
		\
		else if( TYPE & TRACE_TYPE_ERROR ) \
			fprintf(stderr, "%s %s: " FMT, __FILE__, __func__ , ## args);\
		\
		else if( TYPE & TRACE_TYPE_PERROR ) \
		{\
			char __data__[256];                                             \
			memset(__data__,0,256);                                         \
			snprintf(__data__,255,"%s %s [%d]: " FMT, __FILE__, __func__,__LINE__, ## args);   \
			perror(__data__);                                               \
		}\
	}

#define TRACEE( FMT, args... ) TRACE( TRACE_TYPE_ERROR, g_verbose_level, FMT, ## args )
#define TRACEPE( FMT, args... ) TRACE( TRACE_TYPE_PERROR, g_verbose_level, FMT, ## args )
#define TRACEI( FMT, args... ) TRACE( TRACE_TYPE_INFO, g_verbose_level, FMT, ## args )
#define TRACEV( FMT, args... ) TRACE( TRACE_TYPE_VARS, g_verbose_level, FMT, ## args )
#define TRACEF( FMT, args... ) TRACE( TRACE_TYPE_FUNC, g_verbose_level, FMT, ## args )
#define TRACER( FMT, args... ) TRACE( TRACE_TYPE_RAW, g_verbose_level, FMT, ## args )
#define TRACED( FMT, args... ) TRACE( TRACE_TYPE_DEBUG, g_verbose_level, FMT, ## args )

#endif				//_ADAL_COMMON_H
