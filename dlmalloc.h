/*==========================================================================
//
// This file is part of the MemPool library.
// Copyright (C) 2017 Bernd Edlinger
//
// MemPool is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License.  However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v3.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
//
//==========================================================================*/
#ifndef MEMPOOL_DLMALLOC_H
#define MEMPOOL_DLMALLOC_H

/*==========================================================================
//
//      dlmalloc.h
//
//      Interface to the port of Doug Lea's malloc implementation
//
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    jlarmour
// Contributors: Bernd Edlinger
// Date:         2000-06-18
// Purpose:      Define standard interface to Doug Lea's malloc implementation
// Description:  Doug Lea's malloc has been ported to eCos. This file provides
//               the interface between the implementation and the standard
//               memory allocator interface required by eCos
// Usage:        #include <cyg/memalloc/dlmalloc.hxx>
//
//
//####DESCRIPTIONEND####
//
//==========================================================================*/

/* CONFIGURATION */

/* INCLUDES */

#include "mpcommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
/* Constructor: gives the base and size of the arena in which memory is */
/* to be carved out, note that management structures are taken from the */
/* same arena. */
void Cyg_dlmalloc_init( cyg_uint8 *base, cyg_uint32 size );

/* get some memory, return NULL if none available */
cyg_uint8 * Cyg_dlmalloc_try_alloc( cyg_uint32 size );

/* resize existing allocation, if oldsize is non-NULL, previous */
/* allocation size is placed into it. If previous size not available, */
/* it is set to 0. NB previous allocation size may have been rounded up. */
/* Occasionally the allocation can be adjusted *backwards* as well as, */
/* or instead of forwards, therefore the address of the resized */
/* allocation is returned, or NULL if no resizing was possible. */
/* Note that this differs from ::realloc() in that no attempt is */
/* made to call malloc() if resizing is not possible - that is left */
/* to higher layers. The data is copied from old to new though. */
/* The effects of alloc_ptr==NULL or newsize==0 are undefined */
cyg_uint8 * Cyg_dlmalloc_resize_alloc( cyg_uint8 *alloc_ptr, cyg_uint32 newsize,
                  cyg_uint32 *oldsize );

/* free the memory back to the pool */
/* returns true on success */
void Cyg_dlmalloc_free( cyg_uint8 *ptr );

/* Get memory pool status */
/* flags is a bitmask of requested fields to fill in. The flags are */
/* defined in common.h */
void Cyg_dlmalloc_get_status( cyg_uint32 flags,
                  struct Cyg_Mempool_Status *status );

#ifdef __cplusplus
}
#endif

#endif /* ifndef MEMPOOL_DLMALLOC_H */
/* EOF dlmalloc.h */
