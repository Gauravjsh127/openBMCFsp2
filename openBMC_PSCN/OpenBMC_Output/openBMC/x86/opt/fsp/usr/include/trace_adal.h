/******************************************************************************
 * IBM Confidential
 *
 * Licensed Internal Code Source Materials
 *
 * IBM Flexible Support Processor Licensed Internal Code
 *
 * (c) Copyright IBM Corp. 2004, 2009, 2012
 *
 * The source code is for this program is not published or otherwise divested
 * of its trade secrets, irrespective of what has been deposited with the
 * U.S. Copyright Office.
 *****************************************************************************
 * \file trace_adal.h
 * \brief Contains header data for trace component..
 *
 *  The trace component allows an application to trace its execution into
 *  circular buffers (like a flight recorder) with low performance and
 *  memory usage impact.  This implementation focuses on the Linux operating
 *  system running on embedded controllers.
 *
 * \note Please see the document trace_doc.lyx for full documentation on this
 * \note component.
 *****************************************************************************/


#ifndef _TRACE_ADAL_H
#define _TRACE_ADAL_H

#include <sys/types.h>
#include <sys/uio.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <asm/ioctl.h>

/*----------------------------------------------------------------------------*/
/* From trace_dd.h, which can not be included in adals.                       */
/* Removed doxoygen tags, because they would be redundant.                    */
/*----------------------------------------------------------------------------*/

#define TRACE_IOCTL_MAGIC 'h' /*!< random magic number for ioctl calls */

#define  TRACE_SET_BUFFER           _IOWR(TRACE_IOCTL_MAGIC,0,int)
#define  TRACE_GET_BUFNAMES         _IOWR(TRACE_IOCTL_MAGIC,1,int)
#define  TRACE_DO_TRACE             _IOWR(TRACE_IOCTL_MAGIC,2,int)
#define  TRACE_READ_BUFFER          _IOWR(TRACE_IOCTL_MAGIC,3,int)
#define  TRACE_SET_DEBUG            _IOWR(TRACE_IOCTL_MAGIC,4,int)
#define  TRACE_SET_INT_TRACE        _IOWR(TRACE_IOCTL_MAGIC,5,int)
#define  TRACE_UNSET_INT_TRACE      _IOWR(TRACE_IOCTL_MAGIC,6,int)
#define  TRACE_DELETE_BUFFER        _IOWR(TRACE_IOCTL_MAGIC,7,int)
#define  TRACE_CLEAR_BUFFERS        _IOWR(TRACE_IOCTL_MAGIC,8,int)
#define  TRACE_DO_TRACEV            _IOWR(TRACE_IOCTL_MAGIC,9,int)
#define  TRACE_SET_CONSOLE          _IOWR(TRACE_IOCTL_MAGIC,10,int)
#define  TRACE_SET_PIPE             _IOWR(TRACE_IOCTL_MAGIC,11,int)
#define  TRACE_PIPEENABLE	    _IOWR(TRACE_IOCTL_MAGIC,12,int) //Where could this be passed in (or are kernel functions unused)
#define  TRACE_DFR_BUFFER	    _IOWR(TRACE_IOCTL_MAGIC,13,int) //Enable this?
#define  TRACE_READ_DELTA           _IOWR(TRACE_IOCTL_MAGIC,14,int)
#define  TRACE_SET_THRESHOLD	    _IOWR(TRACE_IOCTL_MAGIC,15,int)
#define  TRACE_READ_RECENT	    _IOWR(TRACE_IOCTL_MAGIC,18,int)

/*!@brief Maximum size of component name
 * @note Make sure to also change in include/linux/trac.h -
 *   TRACER_FSP_TRACE_NAME_SIZE
*/
#define TRACE_MAX_COMP_NAME_SIZE 16

#define TRACE_DEBUG_ON  1		/*!<Set to this when debug trace on */
#define TRACE_DEBUG_OFF  0		/*!<Set to this when debug trace off */
#define TRACE_DEBUG  1			/*!<Pass this when trace is debug */
#define TRACE_FIELD 0			/*!<Pass this when trace is field */

#define TRACE_COMP_TRACE	0x434F		/*!<Identifies trace as a component trace (printf)*/
#define TRACE_BINARY_TRACE	0x4249		/*!<Identifies trace as a binary trace */
#define TRACE_INTERNAL_BLOCKED	0xFF42		/*!<Identifies trace as an dd internal trace */

#define TRACE_BUFFER_VERSION		1	/*!<Trace buffer version */
#define TRACE_BUFFER_BINARY_VERSION	2	/*!<Trace buffer version when collected by fsp-trace from pipe */

#define TRACE_DEFAULT_TD  0		/*!<Default trace descriptor */
#define TRACE_PIPE_BUF_SIZE (4 * 4096)	/*!<Size of pipe used by trace daemon */

#define TRACE_MAX_PROC_COPY 3000	/*!<Maximum data that can be copied at one time through /proc (3072 to be exact) */


/*
 * Parsing and output modifier flags
 */
#define TRACE_MIX_BUFFERS		1
/*!< 		When multiple buffers are given the traces of all buffers are sorted by timestamp and printed as one list. 
 * 		If this flag is not given the traces are printed separatly for each trace buffers (i.e. grouped by buffer). 
 */

#define TRACE_PREPEND_BUFFERNAME	2
/*!<		Show the name of a trace buffer for each trace. The buffer name will be inserted between timestamp and trace text. 
 *		Only one of TRACE_APPEND_BUFFERNAME and TRACE_PREPEND_BUFFERNAME can be given.
 */

#define TRACE_APPEND_BUFFERNAME		4
/*!<		Show the name of a trace buffer for each trace. The buffer name will be appended at the end of the line
 *		(after	trace	text).	Only one of TRACE_APPEND_BUFFERNAME and TRACE_PREPEND_BUFFERNAME can be given.
 */

#define TRACE_TIMEOFDAY			8
/*!<		When set timestamps are translated to timeofday values (date/time). This needs "timeref" to be given. 
 *		If timeref is not given the timestamps are treated as if the PPC timebase counter was started at epoch time
 *		(i.e. the printed timestamp will be the time since FSP boot time).
 */

#define TRACE_SET_TIMEOFDAY		16
/*!<		If a TIMEREF trace is found in a trace buffer and timeref is a valid
 *		pointer the values from the TIMEREF trace are written to timeref. This flag is independent of TRACE_TIMEOFDAY.
 */

#define TRACE_FILENAME			32
/*!<		Show the name of the source file that contains the trace statement for each trace.
 *		(at the end of the line, after buffer name if this is printed too).
 */

#define TRACE_VERBOSE			64	/*!<some messages are printed to STDERR. */
#define TRACE_IGNORE_VERSION		128
#define TRACE_OVERWRITE			256
#define TRACE_BINARY			512
#define TRACE_DONTSTOP			1024
/*!<		When this is set trace pipe isn't turned off after pipe read
 */

#define TRACE_TIME_STRFTIME			2048
/*!<		strftime time format flag
 */



#define TRACE_TID_IRQ			(1<<31)
/*!< MSB of tid field is used as trace-in-irq flag
 */

#define TRACE_TID_TID(tid)		((tid) & ~(TRACE_TID_IRQ))

/*
 * !@enum Will use this to hold line number of trace.
 *
 */
 enum trace_line_num     { trace_line };

/*
 * !@struct Structure is put at beginning of all trace buffers
 */
struct trace_buf_head_v1 {
	uint8_t ver;         /*!< version of this struct (1)                      */
	uint8_t hdr_len;     /*!< size of this struct in bytes                    */
	uint8_t time_flg;    /*!< meaning of timestamp entry field                */
	uint8_t endian_flg;  /*!< flag for big ('B') or little ('L') endian       */
	char comp[TRACE_MAX_COMP_NAME_SIZE];
	uint32_t size;       /*!< size of buffer, including this struct           */
	uint32_t times_wrap; /*!< how often the buffer wrapped                    */
	uint32_t next_free;  /*!< offset of the byte behind the latest entry      */
};

struct trace_buf_head_v2 {
	uint8_t ver;         /*!< version of this struct (1)                      */
	uint8_t hdr_len;     /*!< size of this struct in bytes                    */
	uint8_t time_flg;    /*!< meaning of timestamp entry field                */
	uint8_t endian_flg;  /*!< flag for big ('B') or little ('L') endian       */
	char comp[TRACE_MAX_COMP_NAME_SIZE];
	uint32_t size;       /*!< size of buffer, including this struct           */
	uint32_t times_wrap; /*!< how often the buffer wrapped                    */
	uint32_t next_free;  /*!< offset of the byte behind the latest entry      */
	uint32_t extracted;
	uint32_t te_count;
};

/*
 * !@struct Type definition for trace_buf_head
 */
typedef struct trace_buf_head_v2 trace_buf_head_t;


/*!
 * @struct Device driver fills in this structure for each trace entry.
 * It will put this data first in the trace buffer.
 */
typedef struct trace_entry_stamp {
    uint32_t tbh;        /*!< timestamp upper part                            */
    uint32_t tbl;        /*!< timestamp lower part                            */
    uint32_t tid;        /*!< process/thread id                               */
} trace_entry_stamp_t;


/*
 * !@struct Structure is used by adal app. layer to fill in trace info.
 */
typedef struct trace_entry_head {
    uint16_t length;     /*!< size of trace entry                             */
    uint16_t tag;        /*!< type of entry: xTRACE xDUMP, (un)packed         */
    uint32_t hash;       /*!< a value for the (format) string                 */
    uint32_t line;       /*!< source file line number of trace call           */
    uint32_t args[0];    /*!< trace args or data of binary trace              */
} trace_entry_head_t;


/*
 * !@struct Structure is used to return current components tracing
 */
typedef struct trace_buf_list {
    char name[TRACE_MAX_COMP_NAME_SIZE]; /*!< component name                  */
    size_t size;                         /*!< size of component trace buffer  */
} trace_buf_list_t;


typedef int trace_desc_t;	//Type definition for users trace descriptor data type
typedef int tracDesc_t;		//Type definition for older trace descriptor type
typedef unsigned long trace_strings_t;	/* type for trace strings */


/*
 * !@typedef Will use this to hold hash value.
 *
 */
typedef uint32_t trace_hash_val;


/*
 * !@struct Structure to use when using ioctl call TRACE_SET_BUFFER
 */
typedef struct trace_set_buffer {
    char *comp;          /*!< component name                                  */
    size_t size;         /*!< requested size of trace buffer                  */
    trace_desc_t *td;    /*!< will put new trace descriptor here              */
} trace_set_buffer_t;


/*
 * !@struct Structure to use when using ioctl call TRACE_DO_TRACE
 */
typedef struct trace_do_trace {
    trace_desc_t td;     /*!< trace descriptor of component buffer to trace to*/
    size_t size;         /*!< size of the entry to trace                      */
    int debug;           /*!< #TRACE_DEBUG if debug, #TRACE_FIELD if field    */
    const void *data;    /*!< pointer to data to trace                        */
    size_t size2;        /*!< size of additional data block (might be 0)      */
    const void *data2;   /*!< pointer to additional data block                */
} trace_do_trace_t;


/*
 * @brief Structure to use when using ioctl call TRACE_DO_TRACEV
 */
typedef struct trace_do_tracev {
    trace_desc_t td;     /*!< trace descriptor of component buffer to trace to*/
    int level;           /*!< debug level of trace                            */
    size_t size;         /*!< size of the entry to trace                      */
    const struct trace_iovec *iovec;    /*!< pointer to data to trace         */
} trace_do_tracev_t;

/*
 * @brief Part of struct trace_do_tracev
 */
typedef struct trace_iovec {
    const void *base;     /* pointer to the data block */
    size_t size;    /* size of the data block */
    int fromuser;   /* 1 == data in user space (IOW "base" comes from user space) */
} trace_iovec_t;

/*
 * @brief Structure to use when using ioctl call TRACE_READ_BUFFER
 */
typedef struct trace_read_buffer {
    char *comp;          /*!< component name of buffer to read                */
    size_t size;         /*!< number of bytes to read from buffer             */
    void *data;          /*!< place to put the data                           */
} trace_read_buffer_t;

/*
 * @brief Structure to use when using ioctl call TRACE_GET_BUFNAMES
 */
typedef struct trace_getbufs {
    size_t size;            /*!< number of buffer names to retrieve           */
    trace_buf_list_t *list; /*!< place to put the buffer name and size        */
} trace_getbufs_t;

/* struct for time */
struct trace_tbtime {
	uint32_t high;
	uint32_t low;
};

/* struct for timeref trace */
typedef struct trace_timeref {
	struct timeval tod;	// time of day - sec/usec
	struct trace_tbtime time_stamp;	// timebase - high/low
	uint32_t frequency;	// timebase frequency (MHz)
} trace_timeref_t;


/*----------------------------------------------------------------------------*/
/* Constants                                                                  */
/*----------------------------------------------------------------------------*/
#define  TRACE_FULL_DEVICE_NAME "/dev/trace/0"
#define  TRACE_PROC_DEVICE_NAME "/proc/fsptrace/device"
#define  TRACE_PIPE_NAME        "/proc/fsptrace/pipe"

/* additional trace types */

/* only define if not defined by trace_dd.h (make different versions of
 * these files compatible). check only for one define instead of all */
#ifndef TRACE_FIELDTRACE

/*!< a component trace of type field (non-debug): x4654 = "FT" */
#define TRACE_FIELDTRACE 0x4654
/*!< a component trace of type debug: x4454 = "DT" */
#define TRACE_DEBUGTRACE 0x4454
/*!< a binary trace of type field (non-debug): x4644 = "FD" */
#define TRACE_FIELDBIN 0x4644
/*!< a binary trace of type debug: x4644 = "DD" */
#define TRACE_DEBUGBIN 0x4444
/*!< a string trace of type field (non-debug): x4653 = "FS" */
#define TRACE_FIELDSTRING 0x4653
/*!< a string trace of type debug: x4453 = "DS" */
#define TRACE_DEBUGSTRING 0x4453

#endif

/*----------------------------------------------------------------------------*/
/* Return Codes                                                               */
/*----------------------------------------------------------------------------*/

/*!
 * \defgroup retCodes Return Codes
 *
 * Errno will be set for all errors.  Here are possible errno values:
 * - E2BIG: The given buffer size exceeds the maximum size.
 * - ENOMEM: Not enough memory to allocate the buffer.
 * - EAGAIN: Trace buffer already exists and size requested is larger then it.
 * - ENODEV: No trace device driver is loaded in system.
 * - EFAULT: Unable to copy user data into kernel space.
 * - EINVAL: Trace data pointer is invalid.
 * - ENOENT: No trace buffer exitst for component.
 * - EACCESS: Permission denied.
 
 * @{
 */

/* trace_adal_init_fd             -1 -> -9 */
/*!<
 * Failed to open device, make sure device driver is in kernel.
 */
#define TRACE_INIT_FD_ERR                  -1


/* trace_adal_init_buffer        -10 -> -19 */

/*!<
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_INIT_BUFF_IOCTL_ERR          -10

/*!<
 * Invalid component name.
 */
#define TRACE_INIT_BUFF_NAME_ERR           -11


/* !< trace_adal_setdebug/console   -20 -> -29 
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_SETDEBUG_IOCTL_ERR           -20
#define TRACE_SETCONSOLE_IOCTL_ERR         -20
#define TRACE_SETPIPE_IOCTL_ERR            -20

/*!<
 * Invalid parameter passed to set debug function.
 */
#define TRACE_SETDEBUG_INV_PARM_ERR        -21
#define TRACE_SETCONSOLE_INV_PARM_ERR      -21
#define TRACE_SETPIPE_INV_PARM_ERR         -21


/*!< trace_adal_write              -30 -> -39 */
/*
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_WRITE_IOCTL_ERR              -30
/*!<
 * trace not initialized yet
 */
#define TRACE_WRITE_NOT_INIT               -31


/* trace_adal_free               -40 -> -49 */


/*!<trace_adal_read               -50 -> -59
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_READ_IOCTL_ERR               -50


/*!<trace_adal_getbufs            -60 -> -69
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_GETBUFS_IOCTL_ERR            -60


/*!<trace_adal_delete_buffer      -70 -> -79
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_DELETE_IOCTL_ERR             -70


/* trace_adal_hash               -80 -> -89 */
/* trace_adal_copy_string        -90 -> -99 */

/*!< trace_adal_clear_buffs       -100 -> -109
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_CLEAR_IOCTL_ERR             -100

/*!< trace_adal_write_all      -110 -> -119
 * IOCTL call failed, errno will have more detail.
 */
#define TRACE_WRITE_ALL_IOCTL_ERR             -110
#define TRACE_THREAD_LOCK_FAIL                -111
#define TRACE_THREAD_UNLOCK_FAIL              -112
#define TRACE_WRITE_ALL_BAD_TD                -113


/*!<trace_adal_read_partial     -120 -> -129
 * Invalid component name.
 * Not used anymore, kept not to break old code.
 */
#define TRACE_READ_PARTIAL_ERR                -120

/*@}*/

/*----------------------------------------------------------------------------*/
/* Types                                                                      */
/*----------------------------------------------------------------------------*/

/*!@struct a trace entry with args
 * the args can be accessed via h.args[..] too
 */
struct trace_entry {
    trace_entry_head_t h;
    uint32_t args[9];
};
typedef struct trace_entry trace_entry_t;

/*----------------------------------------------------------------------------*/
/* Globals                                                                    */
/*----------------------------------------------------------------------------*/
/**
 * @brief Used by hash function to mix up hash values
*/
#define TRAC_HASH_MIX(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8);  \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5);  \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initialize a trace buffer for a component.
 *
 * @param td Device driver will assign caller a trace descriptor.
 * @param comp Pointer to 16 character null terminated string.
 * @param size Requested buffer size.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_INIT_BUFF_IOCTL_ERR device driver refused to create buffer
 * @retval #TRACE_INIT_BUFF_NAME_ERR buffer name was too long, a buffer with the
           name "BADN" was created instead
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_init_buffer(trace_desc_t *,const char *,const size_t);

/*!
 * @brief Set trace debug level
 *
 * @param td Assigned trace descriptor.
 * @param level If 0 only field traces will be active. If > 0 debug traces
 *              with level <= 'level' will be active.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_SETDEBUG_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_SETDEBUG_INV_PARM_ERR second parm must be TRACE_DEBUG_ON or TRACE_DEBUG_OFF
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_setdebug(const trace_desc_t, const int32_t);

/*!
 * @brief Set trace percentage (floored to the closest multiple of 10) level
 *
 * @param level [1-99] corresponds to 1%-99%.  But note that 70-79 = 70%.
 *
 * @return 0 for success.  Errno set on error
 */
int32_t trace_adal_set_threshold(const int32_t);

/*!
 * @brief Set trace console level
 *
 * @param td Assigned trace descriptor.
 * @param level If -1 no traces will be written to console. If 0 only field
 *              traces will be written to console. If > 0 debug traces
 *              with level <= 'level' will be written to console too.
 *              Only active debug traces will be shown on console
 *              (cf. trace_adal_setdebug).
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_SETCONSOLE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_SETCONSOLE_INV_PARM_ERR second parm must be -1, 0 or 1
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_setconsole(const trace_desc_t, const int32_t);

/*!
 * @brief Set trace pipe level
 *
 * @param td Assigned trace descriptor.
 * @param level If -1 no traces will be written to pipe. If 0 only field
 *              traces will be written to pipe. If > 0 debug traces
 *              with level <= 'level' will be written to pipe too.
 *              Only active debug traces will be written to pipe
 *              (cf. trace_adal_setdebug).
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_SETCONSOLE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_SETCONSOLE_INV_PARM_ERR second parm must be -1, 0 or 1
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_setpipe(const trace_desc_t, const int32_t);

/* as above, but with buffer names */
int32_t trace_adal_setdebug_name(const char *name, const int32_t level);
int32_t trace_adal_setconsole_name(const char *name, const int32_t level);
int32_t trace_adal_setpipe_name(const char *name, const int32_t level);

/*!
 * @brief Write some data to trace buffer.
 *
 * @param td Assigned trace descriptor.
 * @param size Size of data.
 * @param debug Is it a debug trace or field.
 * @param data Data to write to buffer.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_WRITE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_WRITE_NOT_INIT trace device isn't opened, call trace_adal_init_buffer before
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_write(const trace_desc_t, const size_t,
                         const int32_t,const void *);

/*!
 * @brief Write some data to trace buffer.
 *
 * @param td Assigned trace descriptor.
 * @param debug Is it a debug trace or field.
 * @param size Size of data.
 * @param data Data to write to buffer.
 * @param size2 Size of second data block.
 * @param data2 Second data block to write to buffer.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_WRITE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_write2(const trace_desc_t, const int32_t,
                         const size_t,const void *,const size_t,const void *);

/*!
 * @brief Write some data to trace buffer.
 *
 * @param td Assigned trace descriptor.
 * @param debug Is it a debug trace or field.
 * @param count Number of elements in iov
 * @param iov Pointer to trace_iovec list
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_WRITE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_WRITE_NOT_INIT trace device isn't opened, call trace_adal_init_buffer before
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_writev(const trace_desc_t, const int32_t,
                         const size_t, const struct trace_iovec * iov);

/*!
 * @brief Write some data to trace buffer.
 *
 * @param td Assigned trace descriptor.
 * @param debug Is it a debug trace or field.
 * @param line source file line number
 * @param nargs number of uint32_t args
 * @param hash hash value of printf string
 * @param p1 - p9 params to store in the trace
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_WRITE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_write_ints(const trace_desc_t i_td, const int32_t i_debug,
        uint32_t line, int nargs, uint32_t hash, uint32_t p1, uint32_t p2,
	uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, 
	uint32_t p8, uint32_t p9);

int32_t trace_adal_write_ints5(const trace_desc_t i_td, const uint32_t i_dln,
        uint32_t hash, uint32_t p1, uint32_t p2,
	uint32_t p3, uint32_t p4, uint32_t p5);
int32_t trace_adal_write_ints9(const trace_desc_t i_td, const uint32_t i_dln,
        uint32_t hash, uint32_t p1, uint32_t p2,
	uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7, 
	uint32_t p8, uint32_t p9);


/*!
 * @brief Let device driver know that you are done tracing for this comp.
 *
 * @param td Free this trace descriptor
 *
 * @return Always return 0
 */
int32_t trace_adal_free(trace_desc_t *io_td);

/*!
 * @brief Read data from a trace buffer.
 * trace_adal_read - reads from beginning of buffer
 * trace_adal_read_diff - reads from last extract mark.
 * trace_adal_read_recent - read from the end of trace buffer.
 *
 * Note: extract mark only set with trace_adal_read_diff
 *
 * @param comp Component names buffer you wish to read..
 * @param size Size of data to read.
 * @param data_ptr Place to put the data.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_READ_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_read(const char *, const size_t, void *);
int32_t trace_adal_read_diff(const char *, const size_t, void *);
int32_t trace_adal_read_recent(const char *, const size_t, void *);

/*!
 * @brief Read current buffers and sizes from device driver.
 *
 * @param num Number of instances of the first parameter being passed.
 * @param data_ptr Will put all component names and sizes into this parameter.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_GETBUFS_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_getbufs(const size_t, trace_buf_list_t *);

/*!
 * @brief Delete trace buffer associated with parameter value.
 *
 * @param comp Name of component that is to have memory buffer deleted.
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_DELETE_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_delete_buffer(const char *);

/*!
 * @brief Hash the string value into 4 byte hash..
 *
 * @param printf_string Pointer to printf string
 * @param seed Seed value for hash function
 *
 * @return Always return 4 byte hash value
 */
trace_hash_val trace_adal_hash(const char *, const uint32_t);

/*!
 * @brief Copy the entire string into our args entry.
 *
 * @param comp Name of component that is to have memory buffer deleted.
 * @param data_ptr Place to copy the data to.
 * @param counter Location counter to increment.
 *
 * @return Always return 0
 */
int32_t trace_adal_copy_string(const char *,void *,uint32_t*);

/*!
 * @brief
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_CLEAR_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 */
int32_t trace_adal_clear_buffs(void);

/*!
 * @brief Write trace data (can handle all data types)
 *
 * @return 0 for success, negative value for failure.
 * @retval #TRACE_WRITE_ALL_IOCTL_ERR error from device driver, errno set
 * @retval #TRACE_WRITE_NOT_INIT trying to trace without device driver
 * @retval #TRACE_THREAD_LOCK_FAIL error locking thread lock
 * @retval #TRACE_THREAD_UNLOCK_FAIL error unlocking thread lock
 * @retval #TRACE_INIT_FD_ERR cannot open trace device (module not loaded?), errno set
 * @retval #TRACE_WRITE_ALL_BAD_TD bad trace descriptor
 */
int32_t trace_adal_write_all(const trace_desc_t i_td,const trace_hash_val i_hash,
                             const char *i_fmt,
                             const uint32_t i_line, const int32_t i_type,...)
        __attribute__ ((format (printf, 3, 6)));


/************* interface for parsing buffers *****************/

/*!
 * @brief Reads a trace string file and adds the information to "strings".
 *
 * @param strings Destination.
 * @param infile File where the stringfile will be read from.
 * @param flags Can be:
 *	TRACE_IGNORE_VERSION If set the stringfile is not checked for a magic header line.
 *		If not set and the check fails the file is not read and an error is returned
 *	TRACE_OVERWRITE If a string read from infile has the same id as a string that is already in
 *		strings but the string itself is different, the one from infile replaces
 *		(overwrites) the conflicting one in strings. Without this flag such a conflict
 *		is treated as error and an error returned.
 *	TRACE_VERBOSE When this is set some messages are printed to STDERR. The messages eg. tell
 *		about checking the file header (TRACE_CHECK_VERSION), about string
 *		conflicts and a summary about how much strings have been read. There is no
 *		formal definition of these messages.
 *
 * @return on success a pointer to a trace_strings_t structure will be returned. 
 *		On error 0 will be returned and errno set accordingly.
 */
trace_strings_t trace_adal_read_stringfile(trace_strings_t strings,
					    const char *infile, int flags);


/*!
 * @brief Deallocates the memory allocated with trace_adal_read_stringfile() for "strings".
 *
 * @param strings Has to point to a trace_string_t structure that holds information about trace format strings.
 */

void trace_adal_free_strings(trace_strings_t strings);


/*!
 * @brief Parses a trace buffer, splits it into the trace entries and writes the traces formatted to the file "outfile".
 *
 * @param vec Points to a list of "vecsize" number of struct iovec elements with pointers to trace buffers and their sizes.
 * @param outfile File descriptor where the traces should be written to using the "write" system call.
 *	If outfile is "-1" no output is generated. This can be used to check if a buffer is valid 
 *	and to look for timref values from a TIMEREF trace.
 * @param strings Has to point to a trace_string_t structure that holds information about trace format strings.
 *	This structure has to be created with trace_adal_read_stringfile().
 * @param timeref Has to contain a pointer to a trace_timeref_t structure (cf. Section 1.5.4.2, "Trace with time reference
 *	and timebase information") if one of the flags TRACE_TIMEOFDAY and TRACE_SET_TIMEOFDAY is set.
 *	This structure contains a pair of time values and the timestamp frequency. These are used to translate the
 *	timestamp of the traces into timeofday values. If the timeref is 0 timestamp translation is only possible if a
 *	trace buffer contains a TIMEREF trace entry. Traces read and formatted prior to reading this trace entry are
 *	shown with untranslated timestamps.
 * @param flags Defines the output. It is the "OR"'ed value of some of the following flags:
 *	- TRACE_MIX_BUFFERS
 * 		When multiple buffers are given the traces of all buffers are sorted by timestamp and printed as one list. 
 * 		If this flag is not given the traces are printed separatly for each trace buffers (i.e. grouped by buffer). 
 *	- TRACE_PREPEND_BUFFERNAME
 *		Show the name of a trace buffer for each trace. The buffer name will be inserted between timestamp and trace text. 
 *		Only one of TRACE_APPEND_BUFFERNAME and TRACE_PREPEND_BUFFERNAME can be given.
 *	- TRACE_APPEND_BUFFERNAME
 *		Show the name of a trace buffer for each trace. The buffer name will be appended at the end of the line
 *		(after	trace	text).	Only one of TRACE_APPEND_BUFFERNAME and TRACE_PREPEND_BUFFERNAME can be given.
 *	- TRACE_TIMEOFDAY
 *		When set timestamps are translated to timeofday values (date/time). This needs "timeref" to be given. 
 *		If timeref is not given the timestamps are treated as if the PPC timebase counter was started at epoch time
 *		(i.e. the printed timestamp will be the time since FSP boot time).
 *	- TRACE_SET_TIMEOFDAY If a TIMEREF trace is found in a trace buffer and timeref is a valid
 *		pointer the values from the TIMEREF trace are written to timeref. This flag is independent of TRACE_TIMEOFDAY.
 *	- TRACE_TIME_STRFTIME Will be used to set time in strftime time format.
 *		Time format passed by g_time_format external variable.
 *	- TRACE_FILENAME
 *		Show the name of the source file that contains the trace statement for each trace.
 *		(at the end of the line, after buffer name if this is printed too).
 *	- TRACE_VERBOSE When this is set some messages are printed to STDERR. The messages
 *		eg. tell about the processed trace buffers (version, size ...), number of
 *		traces in them etc. There is no formal definition of these messages.
 *
 *
 * @return on success the number of traces written is returned. On failure a value <0 is returned.
 */
int trace_adal_print_buffers(const struct iovec *vec, size_t vecsize,
			     int outfile, const trace_strings_t strings,
			     trace_timeref_t * timeref, int flags);

/*!
 * @brief Reads traces from the trace device driver's pipe or a file. Writes the traces to the file descriptor outfile
 *	either binary for formatted. The traces will be read from the pipe or the file in chunks to limit memory consumption. 
 *	If the input for this function is a file the whole file will be read and printed. If the input is the trace 
 *	pipe one chunk of data will be read and printed. If the trace pipe buffer isn't full yet the function will sleep 
 *	to wait for the buffer to fill. If the next chunk of data should be read from the pipe the function has to be called again. 
 *	This way the user of this function can handle eg. keyboard input or open a new file to keep the file size below a limit.
 *	If fd contains -1 traces are read from the trace pipe. If fd contains a valid file descriptor traces are read from
 *	this file. The file should have been created with this function with the TRACE_BINARY flag set.
 *
 * @param outfile Is a file descriptor where the traces should be written to using the "write(2)" system call. If out-
 *	file is "-1" no output is generated. This can be used to check if a buffer is valid and to look for timref values
 *	from a TIMEREF trace.
 * @param strings Has to point to a trace_string_t structure that holds information about trace format strings. This struc-
 *	ture can be created and filled with trace_adal_read_stringfile(). strings is ignored if the TRACE_BINARY
 *	flag is set.
 * @param timeref Has to contain a pointer to a trace_timeref_t structure (cf. Section 1.5.4.2, "Trace with time reference
 *	and timebase information") if one of the flags TRACE_TIMEOFDAY and TRACE_SET_TIMEOFDAY is set.
 *	This structure contains a pair of time values and the timestamp frequency. These are used to translate the
 *	timestamp of the traces into timeofday values. If the timeref is 0 timestamp translation is only possible if a
 *	trace buffer contains a TIMEREF trace entry. Traces read and formatted prior to reading this trace entry are
 *	shown with untranslated timestamps.
 * @param flags Defines the output. It is the "OR"'ed value of some of the following flags:
 *	- TRACE_TIME_????? Specifies the format of the timestamp value of the traces. See time_flg
 *		for the possible time flags. At least one of these has to be given for version 2 of the trace pipe. 
 *		For pipe versions 3 and above this flag will not be needed and might even be ignored.
 *	- TRACE_BINARY The traces read from the pipe are not formatted and written in binary format to the file.
 *	- TRACE_PREPEND_BUFFERNAME, TRACE_APPEND_BUFFERNAME The trace pipe always can contain traces from different trace buffers,
 *		trace_adal_print_pipe works always in TRACE_MIX_BUFFERS mode. One of these two flags should be given 
 *		to show the buffer a trace was written to (which will correspond to the component that issued the trace).
 *		Ignored if TRACE_BINARY is set.
 *	- TRACE_TIMEOFDAY When set timestamps are translated to timeofday values (date/time). This
 *		needs timeref to be given. If timeref is not given the timestamps are
 *		treated as if the PPC timebase counter was started at epoch time. Ignored if TRACE_BINARY is set.
 *	- TRACE_SET_TIMEOFDAY If a TIMEREF trace is found in a trace buffer and timeref is a valid
 *		pointer the values from the TIMEREF trace are written to timeref.This flag is independent of TRACE_TIMEOFDAY.
 *	- TRACE_SET_STRFTIME Will be used to set time in strftime time format.
 *		Time format passed by _TIME_FORMAT extern variable.
 *	- TRACE_FILENAME Show the name of the source file that contains the trace statement for
 *		each trace. Ignored if TRACE_BINARY is set.
 *	- TRACE_VERBOSE When this is set some messages are printed to STDERR. The messages
 *		eg. tell about the source for the traces (file/pipe), number of traces read etc. 
 *		There is no formal definition of these messages.
 *
 * @ return on success the number of traces written is returned.  On failure a value <0 is returned.
 */
int trace_adal_print_pipe(int fd, int outfile, trace_strings_t strings,
			  trace_timeref_t * timeref, int flags);

#ifdef __cplusplus
}
#endif

#endif
