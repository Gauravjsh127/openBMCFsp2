/******************************************************************************
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

#ifndef _FSPREBOOT_H_
#define _FSPREBOOT_H_

/******************************************************************************
 *
 * FSP specific resets
 *
 * @brief The FSP system requires special reset types for error data capture
 * and for manufacturing.
 *
 *     1) toolreset needed by manufacturing.
 *     2) pabendpanic causes mcore dump collection during reboot.
 *
 * @file fspreboot.h
 *
 * @author Xiaohan Qin <XQIN@AUSTIN.vnet.ibm.com>
 * @author Kenan Esau <esau1@de.ibm.com>
 *
 *
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>		/* syscall definition */
#include <linux/reboot.h>	/* definition of magic numbers */
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>

#define toolreset()					\
	syscall(SYS_reboot, LINUX_REBOOT_MAGIC1,	\
		LINUX_REBOOT_MAGIC2,			\
		LINUX_REBOOT_CMD_TOOLRESET,		\
		NULL)

#define pabendpanic(panic_str)				\
	syscall(SYS_reboot, LINUX_REBOOT_MAGIC1,	\
		LINUX_REBOOT_MAGIC2,			\
		LINUX_REBOOT_CMD_COREDUMP,		\
		panic_str)


static inline void
sys_pabend(unsigned int t, unsigned int data_a, unsigned int data_b,
	   unsigned int data_c) {
	char str[80];
	size_t len = 80;
	snprintf(str, len, "NEWPABEND %x %x %x %x\n", t, data_a,
		 data_b, data_c);
	str[len - 1] = 0;

#ifndef OCOTEA
	pabendpanic(str);
#endif
}

#ifdef __cplusplus
}
#endif

#endif				/* _FSPREBOOT_H_ */
