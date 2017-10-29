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

#ifndef __VERSION__H__
#define __VERSION__H__

#define VER_TO_MAJOR(x)	(((x) & 0xFF000000) >> 24)
#define VER_TO_MINOR(x)	(((x) & 0x00FF0000) >> 16)
#define VER_TO_PATCH(x)	(((x) & 0x0000FF00) >> 8)

#define MAJOR_TO_VER(x)	((0xFF & (x)) << 24)
#define MINOR_TO_VER(x)	((0xFF & (x)) << 16)
#define PATCH_TO_VER(x)	((0xFF & (x)) << 8)

#define VER(x,y,z) (MAJOR_TO_VER(x) | MINOR_TO_VER(y) | PATCH_TO_VER(z))

#endif				/* __VERSION__H__ */
