/*
 *   Copyright (c) International Business Machines Corp., 2012
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 *    File: spinor.h
 *  Author: Shaun Wetzstein <shaun@us.ibm.com>
 *   Descr: SPI NOR I/O library that uses the Aardvark API
 *    Date: 09/20/2012
 */

#ifndef __SPINOR_H__
#define __SPINOR_H__

#ifdef AARDVARK
#include <stdio.h>
#include <stdint.h>

#include "aardvark.h"
#endif

typedef struct spinor spinor_t;
typedef struct spinor_cmdset spinor_cmdset_t;
typedef struct spinor_geometry spinor_geometry_t;

/* ========================================================================= */

#define SPINOR_ID_SZ			3UL
#define SPINOR_ID_MFG			0
#define SPINOR_ID_MFG_MICRON		0x20
#define SPINOR_ID_MFG_MACRONIX		0xC2

#define SPINOR_ID_TYPE			1
#define SPINOR_ID_TYPE_M25PX		0x71
#define SPINOR_ID_TYPE_N25Q512		0xBA
#define SPINOR_ID_TYPE_N25Q128		0xBB
#define SPINOR_ID_TYPE_MX25U		0x25
#define SPINOR_ID_TYPE_MX66L		0x20

#define SPINOR_ID_CAPACITY		2
#define SPINOR_ID_CAPACITY_64Mb		0x17	/*  64 Mbit */
#define SPINOR_ID_CAPACITY_128Mb	0x18	/* 128 Mbit */
#define SPINOR_ID_CAPACITY_128Mb_MX	0x38	/* 128 Mbit (Macronix) */
#define SPINOR_ID_CAPACITY_256Mb	0x19	/* 256 Mbit */
#define SPINOR_ID_CAPACITY_512Mb	0x20	/* 512 Mbit */
#define SPINOR_ID_CAPACITY_512Mb_MX	0x1A	/* 512 Mbit (Macronix) */

#define SPINOR_STATUS_BUSY		0x01
#define SPINOR_STATUS_WREN		0x02
#define SPINOR_STATUS_BP_0		0x04
#define SPINOR_STATUS_BP_1		0x08
#define SPINOR_STATUS_BP_2		0x10
#define SPINOR_STATUS_TB		0x20
#define SPINOR_STATUS_SRWD		0x80

#define SPINOR_FLAG_STATUS_READY	0x80
#define SPINOR_FLAG_STATUS_ERASE_SUSP	0x40
#define SPINOR_FLAG_STATUS_ERASE	0x20
#define SPINOR_FLAG_STATUS_PROGRAM	0x10
#define SPINOR_FLAG_STATUS_VPP		0x08
#define SPINOR_FLAG_STATUS_PROGRAM_SUSP	0x04
#define SPINOR_FLAG_STATUS_PROTECTION	0x02

struct spinor_geometry {
	uint32_t device_size;		/* device size (in bytes) */

	uint32_t page_size;		/* maximum page size (bytes) */
	uint32_t sector_size;		/* sector size (in bytes) */
	uint32_t subsector_size;	/* subsector size (in bytes) */

#ifdef AARDVARK
	uint32_t page_program_time;	/* page program (per byte) (in us) */

	uint32_t erase_bulk_time;	/* bulk erase time (in us) */
	uint32_t erase_sector_time;	/* sector erase time (in us) */
	uint32_t erase_subsector_time;	/* subsector erase time (in us) */
#endif
} __attribute__((packed));

struct spinor_cmdset {
	uint8_t addr_size;		/* address size (in bytes) */

	uint8_t read_id;		/* configuration */
	uint8_t read_parms;		/* read serial flash parameters */

	uint8_t page_read;			/* read */
	uint8_t page_program;		/* page program */

	uint8_t write_enable;		/* write enable */
	uint8_t write_disable;		/* write disable */

	uint8_t addr4byte_enable;	/* 4-byte address mode enable */
	uint8_t addr4byte_disable;	/* 4-byte address mode disasble */

	uint8_t read_status;		/* read status register */
	uint8_t write_status;		/* write status register */

	uint8_t read_flag_status;	/* read flag status register */
	uint8_t write_flag_status;	/* write flag status register */

	uint8_t erase_bulk;		/* bulk (device) erase */
	uint8_t erase_sector;		/* sector erase */
	uint8_t erase_subsector;	/* subsector erase */

	uint8_t reserved[4];
} __attribute__((packed));

struct spinor {
	const uint8_t chip_id[SPINOR_ID_SZ];
	uint8_t resvd[5];

	spinor_cmdset_t cmd;
	spinor_geometry_t geo;

#ifdef AARDVARK		/* keep these at the end */
	int port;
	Aardvark aa;
#endif
} __attribute__((packed));

typedef enum {
	r_ERROR = 0,
	r_STATUS,
	r_FLAG_STATUS,
} spinor_register_type_t;

#define SPINOR_REGISTER_SZ	4UL
typedef uint8_t spinor_register_t[SPINOR_REGISTER_SZ];

#define SPINOR_ID_MFG		0
#define SPINOR_ID_TYPE		1
#define SPINOR_ID_SIZE		2

#define SPINOR_ID_SZ		3UL
typedef uint8_t spinor_id_t[SPINOR_ID_SZ];

#define SPINOR_PAGE_SZ		256UL
#define SPINOR_SECTOR_SZ	64*1024UL
#define SPINOR_SUBSECTOR_SZ	4*1024UL

/* ========================================================================= */

extern spinor_t *spinor_open(int);
extern int spinor_close(spinor_t *);

extern int spinor_configure(spinor_t *);

extern int spinor_read_id(spinor_t *, spinor_id_t);
extern uint8_t * spinor_id(spinor_t *);
extern const char * spinor_id_mfg(spinor_id_t);
extern const char * spinor_id_type(spinor_id_t);
extern const char * spinor_id_capacity(spinor_id_t);

extern size_t spinor_address_size(spinor_t *);
extern size_t spinor_device_size(spinor_t *);
extern size_t spinor_sector_size(spinor_t *);
extern size_t spinor_subsector_size(spinor_t *);

extern int spinor_write_enable(spinor_t *);
extern int spinor_write_disable(spinor_t *);

extern int spinor_4byte_enable(spinor_t *);
extern int spinor_4byte_disable(spinor_t *);

extern int spinor_read_register(spinor_t *, spinor_register_type_t,
				spinor_register_t);
extern int spinor_write_register(spinor_t *, spinor_register_type_t,
				 const spinor_register_t);

extern int spinor_read_data(spinor_t *, size_t, size_t, void *);
extern int spinor_write_data(spinor_t *, size_t, size_t, const void *);

extern int spinor_write_sector(spinor_t *, size_t, size_t, const void *);
extern int spinor_write_subsector(spinor_t *, size_t, size_t, const void *);
extern int spinor_read_sector(spinor_t *, size_t, size_t, void *);
extern int spinor_read_subsector(spinor_t *, size_t, size_t, void *);

extern int spinor_erase_bulk(spinor_t *);
extern int spinor_erase_sector(spinor_t *, size_t);
extern int spinor_erase_subsector(spinor_t *, size_t);

extern int spinor_reset(spinor_t *);

#endif /* __SPINOR_H_ */
