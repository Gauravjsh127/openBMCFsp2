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

/*!
 * @file flash.h
 * @brief Flash file handling API
 * @details A flash file is a container for one or more
 *          flash image(s).
 * @details For example,
 * @code
 * #include <flash.h>
 * #include <config.h>
 *
 * int main(const int argc, const char * argv[]) {
 *    config_t * cfg = config_new("test.xml");
 *
 *    flash_image_t * img = flash_image_new("test");
 *    flash_image_create(img, cfg->section);
 *
 *    config_delete(cfg);
 *
 *    FILE * out = fopen("test", "w");
 *    flash_image_save(img, out);
 *    flash_image_save(&__eof__, out);
 *
 *    flash_image_delete(img);
 *    fclose(out);
 *
 *    return 0;
 * }
 * @endcode
 * @author Shaun Wetzstein <shaun@us.ibm.com>
 * @date 2010-2011
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include <clib/libclib.h>

#include <clib/list.h>

#include "flash_toc.h"
#include "misc.h"
#include "section.h"

/* ======================================================================= */

#define FLASH_MAJOR   0x02  //!< Major version number
#define FLASH_MINOR   0x00  //!< Minor version number
#define FLASH_PATCH   0x00  //!< Patch version number

#define FLASH_EXT         ".flash"  //!< Flash file extension
#define FLASH_MAGIC   "FLSI"  //!< Flash file magic number
#define FLASH_EOF   "end-of-file" //!< Flash end-of-file

#define FLASH_NAME_SIZE   44  //!< Maximum size, in bytes, of a flash file name

#define FLASH_FLAG_LSB    0x01  //!< Little-endian header data
#define FLASH_FLAG_MSB    0x02  //!< Big-endian header data
#define FLASH_FLAG_EOF    0x80  //!< End-of-file marker

/*!
 * @brief flash header class
 */
struct flash_header
{
  ident_t id;   //!< Identification
  uint64_t toc_off; //!< Table-of-contents file offset (in bytes, from beginning)
  uint32_t toc_sz;  //!< Table-of-contents file size (in bytes)
  char name[FLASH_NAME_SIZE]; //!< Flash file name string, @see FLASH_NAME_SIZE
};
typedef struct flash_header flash_header_t; //!< Alias for the @em flash_header class

/*!
 * @brief flash image class
 */
struct flash_image
{
  flash_header_t hdr; //!< File-specific metadata
  flash_toc_t toc;  //!< Table-of-contents (toc)
};
typedef struct flash_image flash_image_t; //!< Alias for the @em file_image class

extern flash_image_t __eof__; //!< Flash image end-of-file sentinel

/* ======================================================================= */

/*!
 * @brief Allocate and initialize a flash_image object on the heap
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @return non-NULL on success, NULL otherwise
 */
extern flash_image_t * flash_image_new ( const char * name )
/*! @cond */ __nonnull((1)) /*! @endcond */;

/*!
 * @brief Deallocate a flash_image object from the heap
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @return None
 */
extern int flash_image_delete ( flash_image_t * self )
/*! @cond */ __nonnull((1)) /*! @endcond */;

/*!
 * @brief Create a table-of-contents given a section tree
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param sec [in] section object tree
 * @return None
 */
extern int flash_image_create ( flash_image_t * self, section_t * sec )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Save a flash_image object to an output stream
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param out [in] output stream
 * @return None
 */
extern int flash_image_save ( flash_image_t * self, FILE * out )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Load [the next] flash_image object from an input stream
 * @memberof flash_image
 * @param in [in] input stream
 * @return non-NULL on success, NULL otherwise
 */
extern flash_image_t * flash_image_load ( FILE * in )
/*! @cond */ __nonnull((1)) /*! @endcond */;

/*!
 * @brief Tersely list the contents of a flash_image object to stdout
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param out [in] output stream
 * @param name [in] section name regular expression
 * @return non-NULL on success, NULL otherwise
 */
extern int flash_image_list ( flash_image_t * self, FILE * out, const char * name )
/*! @cond */ __nonnull((1, 2, 3)) /*! @endcond */;

/*!
 * @brief Verbosely list the contents of a flash_image object to stdout
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param out [in] output stream
 * @param name [in] section name regular expression
 * @return non-NULL on success, NULL otherwise
 */
extern int flash_image_info ( flash_image_t * self, FILE * out, const char * name )
/*! @cond */ __nonnull((1, 2, 3)) /*! @endcond */;

/*!
 * @brief Copy a flash_image object from input file to output file
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param in [in] input stream
 * @return non-NULL on success, NULL otherwise
 */
extern int flash_image_copy ( flash_image_t * self, FILE * in, FILE * out )
/*! @cond */ __nonnull((1, 2, 3)) /*! @endcond */;

/*!
 * @brief Extract the contents of a flash_image object
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param in [in] input stream
 * @param name [in] section name regular expression
 * @return non-NULL on success, NULL otherwise
 */
extern int flash_image_extract ( flash_image_t * self, FILE * in,
                                 const char * name )
/*! @cond */ __nonnull((1, 2, 3)) /*! @endcond */;

/*!
 * @brief Pretty print the flash_image object to an output stream
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @param out [in] output stream
 * @return None
 */
extern int flash_image_print ( flash_image_t * self, FILE * out )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Return a copy of the flags of a flash_image object
 * @memberof flash_image
 * @param self [in] flash_image object @em self pointer
 * @return None
 */
extern uint8_t flash_image_flags ( flash_image_t * self )
/*! @cond */ __nonnull((1)) /*! @endcond */;

/* ======================================================================= */

/*!
 * @brief Join a set of flash files together to form a new flash file
 * @param path [in] path to the input flash file
 * @param name [in] array of section name regular expression(s)
 * @param name_nr [in] number of entries in the @em name array
 * @return None
 */
extern int flash_join ( const char * path, const char * name[], size_t name_nr )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Split apart flash file contents into separate files
 * @param path [in] path to the input flash file
 * @param name [in] array of section name regular expression(s)
 * @param name_nr [in] number of entries in the @em name array
 * @return None
 */
extern int flash_split ( const char * path, const char * name[], size_t name_nr )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Tersely pretty print the contents of a flash file to stdout
 * @param path [in] path to the input flash file
 * @param name [in] array of section name regular expression(s)
 * @param name_nr [in] number of entries in the @em name array
 * @return None
 */
extern int flash_list ( const char * path, const char * name[], size_t name_nr )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Verbosely pretty print the contents of a flash file to stdout
 * @param path [in] path to the input flash file
 * @param name [in] array of section name regular expression(s)
 * @param name_nr [in] number of entries in the @em name array
 * @return None
 */
extern int flash_info ( const char * path, const char * name[], size_t name_nr )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/*!
 * @brief Extract the contents of a flash file
 * @param path [in] path to the input flash file
 * @param name [in] array of section name regular expression(s)
 * @param name_nr [in] number of entries in the @em name array
 * @return None
 */
extern int flash_extract ( const char * path, const char * name[], size_t name_nr )
/*! @cond */ __nonnull((1, 2)) /*! @endcond */;

/* ======================================================================= */

#endif        /* __FLASH_H__ */
