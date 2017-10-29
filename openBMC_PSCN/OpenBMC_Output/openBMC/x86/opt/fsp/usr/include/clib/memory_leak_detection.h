/*
 * Copyright (c) International Business Machines Corp., 2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Module: memory_leak_detection.h
 *
 *
 * Functions:
 *
 * Description: This module defines macros which should be used in place
 *  						of malloc and free.  These macros default to malloc and
 *  						free, but, when FIND_MEMORY_LEAK is defined, they
 *  						default to functions which wrapper malloc and free to
 *  						track memory allocations and find memory leaks.  Several
 *  						other useful functions are defined as well.
 *
 * Author: B. Rafanello
 *
 * Version 0.0.0.1
 */

#ifndef MEMORY_LEAK_DETECTION_H

#define MEMORY_LEAK_DETECTION_H 1

#if ( FIND_MEMORY_LEAK > 0 )

#warning Memory Leak Detection enabled!

/* ================================================================= */
/* ================================================================= */
/*                                                                   */
/* The following functions are not meant to be called directly!  Use */
/* macros defined after the functions instead of the functions       */
/* themselves.                                                       */
/* ================================================================= */
/* ================================================================= */

/*********************************************************************/
/*                                                                   */
/*   Function Name: MEMORY_func                                      */
/*                                                                   */
/*   Descriptive Name: This function acts as a replacement for       */
/*                     malloc.  It allocates memory (using malloc)   */
/*                     and enters that memory into a tracking        */
/*                     structure so that memory leaks, if any, may   */
/*                     be found.                                     */
/*                                                                   */
/*   Input: size_t sz - The number of bytes to be allocated.         */
/*          unsigned int Alignment - 0 for non-aligned (normal)      */
/*                                   malloc, > 0 to return an        */
/*                                   address aligned on a specific   */
/*                                   memory boundary.  If > 0, then  */
/*                                   Alignment must be a power of 2  */
/*                                   and a multiple of sizeof(void *)*/
/*          void ** Memory_Location - The address of a variable which*/
/*                                   will hold the address of the    */
/*                                   allocated by this function.     */
/*          const char * mod_name - The name of the module from which*/
/*                                  this function was called.        */
/*          const char * func - The name of the function from which  */
/*                              this function was called.            */
/*          const int line - The line number of the code containing  */
/*                           the call to this function.              */
/*                                                                   */
/*   Output: If Success : The function return value will be 0.       */
/*                        *Memory_Location will be set to the address*/
/*                        of the first byte of the user portion of   */
/*                        any memory that was allocated.             */
/*                                                                   */
/*           If Failure : The function return value will be EINVAL or*/
/*                        ENOMEM.  Errors may be reported on stderr. */
/*                        *Memory_Location will be set to NULL.      */
/*                                                                   */
/*   Error Handling: This function will abort if an error is         */
/*                   is detected.  Errors could be lack of memory, or*/
/*                   corruption of the internal structures used to   */
/*                   track allocated blocks of memory.               */
/*                                                                   */
/*   Side Effects: Memory may be allocated, errors may be reported   */
/*                 on stderr.                                        */
/*                                                                   */
/*   Notes:                                                          */
/*                                                                   */
/*********************************************************************/
int MEMORY_func(size_t sz,
		unsigned int Alignment,
		void **Memory_Location,
		const char *mod_name, const char *func, const int line);

/*********************************************************************/
/*                                                                   */
/*   Function Name: MALLOC_func                                      */
/*                                                                   */
/*   Descriptive Name: This function acts as a replacement for       */
/*                     malloc.  It allocates memory (using malloc)   */
/*                     and enters that memory into a tracking        */
/*                     structure so that memory leaks, if any, may   */
/*                     be found.                                     */
/*                                                                   */
/*   Input: size_t sz - The number of bytes to be allocated.         */
/*          const char * mod_name - The name of the module from which*/
/*                                  this function was called.        */
/*          const char * func - The name of the function from which  */
/*                              this function was called.            */
/*          const int line - The line number of the code containing  */
/*                           the call to this function.              */
/*                                                                   */
/*   Output: If Success : The function return value will be non-NULL.*/
/*                                                                   */
/*           If Failure : The function return value will be NULL.    */
/*                        Errors may be reported on stderr.          */
/*                                                                   */
/*   Error Handling: This function will abort if an error is         */
/*                   is detected.  Errors could be lack of memory, or*/
/*                   corruption of the internal structures used to   */
/*                   track allocated blocks of memory.               */
/*                                                                   */
/*   Side Effects: Memory may be allocated, errors may be reported   */
/*                 on stderr.                                        */
/*                                                                   */
/*   Notes:                                                          */
/*                                                                   */
/*********************************************************************/
void *MALLOC_func(size_t sz, const char *mod_name, const char *func,
		  const int line);

/*********************************************************************/
/*                                                                   */
/*   Function Name: FREE_func                                        */
/*                                                                   */
/*   Descriptive Name: This function frees a block of memory being   */
/*                     tracked by this module and removes the block  */
/*                     from its tracking structures.                 */
/*                                                                   */
/*   Input: const void * p - The address of the block of memory to   */
/*                           be freed.                               */
/*          const char * mod_name - The name of the module requesting*/
/*                                  the block of memory be freed.    */
/*          const char * func - The name of the function requesting  */
/*                              the block of memory be freed.        */
/*          const int line - The line number of the line of code in  */
/*                           module calling this function.           */
/*                                                                   */
/*   Output: If Success : None.                                      */
/*                                                                   */
/*           If Failure : Errors may be reported to stderr.          */
/*                                                                   */
/*   Error Handling: This function causes the internal structures    */
/*                   of this module to be checked as part of the     */
/*                   process of freeing the address p.  This may     */
/*                   cause errors to be reported on stderr. If any   */
/*                   errors are found, then the address p may not be */
/*                   freed.                                          */
/*                                                                   */
/*   Side Effects: The block of memory associated with the address p */
/*                 will be freed and available for reallocation.     */
/*                 Also, the memory tracking structures in this      */
/*                 module will undergo a series of checks.           */
/*                                                                   */
/*   Notes: This function was not intended to be called directly but */
/*          rather through the macro FREE.                           */
/*                                                                   */
/*********************************************************************/
void FREE_func(const void *p, const char *mod_name, const char *func,
	       const int line);

/*********************************************************************/
/*                                                                   */
/*   Function Name: Print_Leak_List                                  */
/*                                                                   */
/*   Descriptive Name: This function walks the list of allocated     */
/*                     memory blocks and prints information about    */
/*                     each one.  If this is done at program exit,   */
/*                     the resulting list of memory blocks most      */
/*                     likely represents leaked memory.              */
/*                                                                   */
/*   Input: None.                                                    */
/*                                                                   */
/*   Output: If Success : If there are any memory blocks being       */
/*                        tracked by this module, information about  */
/*                        block still being tracked will be sent to  */
/*                        stderr.                                    */
/*                                                                   */
/*           If Failure : Error messages may be sent to stderr.      */
/*                                                                   */
/*   Error Handling: If errors are detected, then error messages are */
/*                   output on stderr.                               */
/*                                                                   */
/*   Side Effects: The internal structures of this module are checked*/
/*                 for errors with any errors being reported on      */
/*                 stderr.                                           */
/*                                                                   */
/*   Notes:                                                          */
/*                                                                   */
/*********************************************************************/
void Print_Leak_List(void);

/*********************************************************************/
/*                                                                   */
/*   Function Name: Test_Address_Allocation                          */
/*                                                                   */
/*   Descriptive Name: This function tests the specified address to  */
/*                     to see if it lies within an allocated block   */
/*                     tracked by this module.                       */
/*                                                                   */
/*   Input: void * p - The address to be tested.                     */
/*                                                                   */
/*   Output: If Success : If the address p was found, then 0 will be */
/*                        returned if the address is the start of    */
/*                        a block of allocated memory.  If the       */
/*                        address p was found within an allocated    */
/*                        block of memory, then 1 is returned.       */
/*                                                                   */
/*           If Failure : If the address p was NOT found, then 2 is  */
/*                        returned.  If there was an error in the    */
/*                        memory tracking system then 3 will be      */
/*                        returned.                                  */
/*                                                                   */
/*   Error Handling: This function relies on the error handling      */
/*                   built into the Check_Leak_List function and     */
/*                   has no error handling of its own.               */
/*                                                                   */
/*   Side Effects: If the list of memory allocations contains errors */
/*                 then those errors will be detected and reported   */
/*                 on stderr.                                        */
/*                                                                   */
/*   Notes: If NULL is passed in as the address to test, then the    */
/*          integrity of the internal tracking structures will be    */
/*          checked, in which case a return value of 0 signifies     */
/*          that the internal tracking structures have passed the    */
/*          checks and a return value of 3 indicates that errors     */
/*          were found.                                              */
/*                                                                   */
/*********************************************************************/
unsigned int Test_Address_Allocation(void *p);

/*********************************************************************/
/*                                                                   */
/*   Function Name: Duplicate_String                                 */
/*                                                                   */
/*   Descriptive Name: This function duplicates a string.  The memory*/
/*                     allocated for the duplicate is allocated      */
/*                     using the MALLOC_func routine in this module  */
/*                     and is thus tracked by this module.           */
/*                                                                   */
/*   Input: const char * Source - The string to be copied.           */
/*          const char * mod_name - The name of the module containing*/
/*                                  the function which called this   */
/*                                   function.                       */
/*          const char * func - The name of the function calling     */
/*                              this function.                       */
/*          const int line - The line number of the line of code in  */
/*                           module calling this function.           */
/*                                                                   */
/*   Output: If Success : The function return value will be non-NULL */
/*                        and will point to a duplicate of the       */
/*                        string given in Source.                    */
/*                                                                   */
/*           If Failure : The function return value will be NULL.    */
/*                                                                   */
/*   Error Handling: Any errors detected by this function result in  */
/*                   a function return value of NULL.                */
/*                                                                   */
/*   Side Effects: The memory tracking features of this module are   */
/*                 employed to allocate memory for the duplicate     */
/*                 string produced by this funciton.                 */
/*                                                                   */
/*   Notes:                                                          */
/*                                                                   */
/*********************************************************************/
char *Duplicate_String(const char *Source,
		       const char *mod_name, const char *func, const int line);

/*********************************************************************/
/*                                                                   */
/*   Function Name: Realloc_func                                     */
/*                                                                   */
/*   Descriptive Name: This function performs the same function as   */
/*                     the realloc function in the ANSI C library.   */
/*                                                                   */
/*   Input: const void * p - The address of the block of memory to   */
/*                           be reallocated.                         */
/*          size_t size - The size of the memory block to return.    */
/*          const char * mod_name - The name of the module requesting*/
/*                                  the block of memory be freed.    */
/*          const char * func - The name of the function requesting  */
/*                              the block of memory be freed.        */
/*          const int line - The line number of the line of code in  */
/*                           module calling this function.           */
/*                                                                   */
/*   Output: If Success : The function return value will be a pointer*/
/*                        to the new block of memory.                */
/*                                                                   */
/*           If Failure : NULL will be returned and errno will be set*/
/*                        to a non-null error code.                  */
/*                                                                   */
/*   Error Handling: This function causes the internal structures    */
/*                   of this module to be checked.  This may         */
/*                   cause errors to be reported on stderr. If any   */
/*                   errors are found, then the address p may not be */
/*                   freed.                                          */
/*                                                                   */
/*   Side Effects: A new block of memory of size bytes will be       */
/*                 allocated, the contents of the current block will */
/*                 be copied to the new block (at least as much as   */
/*                 will fit, and the current block will be freed.    */
/*                 This will cause internal structures in this module*/
/*                 to be modified accordingly.                       */
/*                                                                   */
/*   Notes: This function was not intended to be called directly but */
/*          rather through the macro REALLOC.                        */
/*                                                                   */
/*          If p is NULL, then this will cause this function to      */
/*          behave like malloc.                                      */
/*                                                                   */
/*          If size is 0, then this will cause this function to      */
/*          behave like free.                                        */
/*                                                                   */
/*********************************************************************/
void *Realloc_func(void *p,
		   size_t size,
		   const char *mod_name, const char *func, const int line);

/* ================================================================= */
/* ================================================================= */
/*                                                                   */
/* Macros to use for accessing the functions in this module.         */
/*                                                                   */
/* ================================================================= */
/* ================================================================= */

#ifdef MALLOC
#undef MALLOC
#endif
#ifdef FREE
#undef FREE
#endif

/* This macro should be used in place of posix_memalign. */
#define POSIX_MEMALIGN(a, b, c )  MEMORY_func( c, b, a, __FILE__, __func__, __LINE__ )

/* This macro should be used in place of the malloc function.             */
#define MALLOC( a )              MALLOC_func( a, __FILE__, __func__, __LINE__ )

/* This macro should be used in place of the free function.               */
#define FREE( a )                FREE_func( a,  __FILE__, __func__, __LINE__ )

/* This macro should be used to print a list of allocated memory blocks.  */
#define PRINT_ALLOCATION_LIST		 Print_Leak_List()

/* This macro should be used to see if an address lies within a block of
   memory being tracked by this module.                                   */
#define ADDRESS_CHECK( p )       Test_Address_Allocation( p )

/* This macro should be used in place of the strdup function. */
#define STRDUP( p )              Duplicate_String( p, __FILE__, __func__, __LINE__ )

/* This macro should be used in place of the realloc function. */
#define REALLOC( p, s )          Realloc_func( p, s, __FILE__, __func__, __LINE__ )

#else

#define POSIX_MEMALIGN(a, b, c ) posix_memalign( a, b, c )
#define MALLOC(s)                malloc(s)
#define FREE(p)                  free(p)
#define PRINT_ALLOCATION_LIST
#define ADDRESS_CHECK( p )       0
#define STRDUP( p )              strdup( p )
#define REALLOC( p, s )          realloc( p, s )

#endif

#endif
