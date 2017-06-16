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
//========================================================================*/
#ifndef MEMPOOL_MPCOMMON_H
#define MEMPOOL_MPCOMMON_H

/*==========================================================================
//
//      mpcommon.h
//
//      Shared definitions used by memory allocators
//
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    jlarmour
// Contributors: Bernd Edlinger
// Date:         2000-06-12
// Purpose:      Shared definitions used by memory allocators
//
//
//####DESCRIPTIONEND####
//
//========================================================================*/

#if defined _WIN32 || defined _WIN32_WCE
#define MEMPOOL_WIN
#else
#define MEMPOOL_POSIX
#endif

#ifdef MEMPOOL_WIN
#define MEMPOOL_API_CALL __stdcall
#else
#define MEMPOOL_API_CALL
#endif

/* TYPE DEFINITIONS */

typedef unsigned char      cyg_uint8;
typedef unsigned int       cyg_uint32;
typedef int       cyg_bool;

/*==========================================================================
// struct Cyg_Mempool_Status is returned by the get_status() method of
// standard eCos memory allocators. After return from get_status(), any
// field of type T may be set to ((T)-1) to indicate that the information
// is not available or not applicable to this allocator.
//========================================================================*/


struct Cyg_Mempool_Status {
    const cyg_uint8 *arenabase; /* base address of entire pool */
    cyg_uint32  arenasize;      /* total size of entire pool */
    cyg_uint32  freeblocks;     /* number of chunks free for use */
    cyg_uint32  totalallocated; /* total allocated space in bytes */
    cyg_uint32  totalfree;      /* total space in bytes not in use */
    cyg_uint32  blocksize;      /* block size if fixed block */
    cyg_uint32  maxfree;        /* size of largest unused block */
    cyg_uint8   waiting;        /* are there any threads waiting for memory? */
    const cyg_uint8 *origbase;  /* address of original region used when pool */
                                /* created */
    cyg_uint32  origsize;       /* size of original region used when pool */
                                /* created */

    /*==========================================================================
    // maxoverhead is the *maximum* per-allocation overhead imposed by
    // the allocator implementation. Note: this is rarely the typical
    // overhead which often depends on the size of the allocation requested.
    // It includes overhead due to alignment constraints. For example, if
    // maxfree and maxoverhead are available for this allocator, then an
    // allocation request of (maxfree-maxoverhead) bytes must always succeed
    // Unless maxoverhead is set to -1 of course, in which case the allocator
    // does not support reporting this information.
    //========================================================================*/

    cyg_uint8   maxoverhead;
    cyg_uint32  maxAllocBytes;
    cyg_uint32  actualAllocBytes;

};

/*==========================================================================
// Flags to pass to get_status() methods to tell it which stat(s) is/are
// being requested
//========================================================================*/

#define CYG_MEMPOOL_STAT_ARENABASE       (1<<0)
#define CYG_MEMPOOL_STAT_ARENASIZE       (1<<1)
#define CYG_MEMPOOL_STAT_FREEBLOCKS      (1<<2)
#define CYG_MEMPOOL_STAT_TOTALALLOCATED  (1<<3)
#define CYG_MEMPOOL_STAT_TOTALFREE       (1<<4)
#define CYG_MEMPOOL_STAT_BLOCKSIZE       (1<<5)
#define CYG_MEMPOOL_STAT_MAXFREE         (1<<6)
#define CYG_MEMPOOL_STAT_WAITING         (1<<7)
#define CYG_MEMPOOL_STAT_ORIGBASE        (1<<9)
#define CYG_MEMPOOL_STAT_ORIGSIZE        (1<<10)
#define CYG_MEMPOOL_STAT_MAXOVERHEAD     (1<<11)

#define CYG_MEMPOOL_STAT_ALL                    \
              ( CYG_MEMPOOL_STAT_ARENABASE      \
              | CYG_MEMPOOL_STAT_ARENASIZE      \
              | CYG_MEMPOOL_STAT_FREEBLOCKS     \
              | CYG_MEMPOOL_STAT_TOTALALLOCATED \
              | CYG_MEMPOOL_STAT_TOTALFREE      \
              | CYG_MEMPOOL_STAT_BLOCKSIZE      \
              | CYG_MEMPOOL_STAT_MAXFREE        \
              | CYG_MEMPOOL_STAT_WAITING        \
              | CYG_MEMPOOL_STAT_ORIGBASE       \
              | CYG_MEMPOOL_STAT_ORIGSIZE       \
              | CYG_MEMPOOL_STAT_MAXOVERHEAD )

#endif /* ifndef MEMPOOL_MPCOMMON_H */
/* EOF mpcommon.h */
