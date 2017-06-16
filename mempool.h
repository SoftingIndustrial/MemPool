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
#ifndef MEMPOOL_MEMPOOL_H
#define MEMPOOL_MEMPOOL_H

#include "mpcommon.h"

#ifdef __cplusplus
extern "C"
{
#endif
cyg_bool  MEMPOOL_API_CALL MemPool_init(cyg_uint32);
void  MEMPOOL_API_CALL MemPool_exit(void);
void  MEMPOOL_API_CALL MemPool_get_status(cyg_uint32, struct Cyg_Mempool_Status*);
void* MEMPOOL_API_CALL MemPool_Memory_Alloc_Func(cyg_uint32);
void* MEMPOOL_API_CALL MemPool_Memory_ReAlloc_Func(void*, cyg_uint32);
void  MEMPOOL_API_CALL MemPool_Memory_Free_Func(void*);

#ifdef __cplusplus
}
#endif

#endif /* ifndef MEMPOOL_MEMPOOL_H */
/* EOF mempool.h */
