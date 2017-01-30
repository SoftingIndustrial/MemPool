// This file is part of the MemPool library.
// Copyright (C) 2017 Free Software Foundation, Inc.
//
// MemPool is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
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
// General Public License v2.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
#include "common.h"

#ifdef MEMPOOL_WIN
#include <windows.h>
#endif

#ifdef MEMPOOL_POSIX
/* The most common include on Linux */
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#endif

#include "dlmalloc.h"
#include "mempool.h"

#ifdef MEMPOOL_POSIX
#define CRITICAL_SECTION pthread_mutex_t
#define INITIALIZE_CRITICAL_SECTION(pcs) pthread_mutex_init(pcs, NULL)
#define DELETE_CRITICAL_SECTION(pcs) pthread_mutex_destroy(pcs)
#define ENTER_CRITICAL_SECTION(pcs) pthread_mutex_lock(pcs)
#define LEAVE_CRITICAL_SECTION(pcs) pthread_mutex_unlock(pcs)
#endif

#ifdef MEMPOOL_WIN
#define INITIALIZE_CRITICAL_SECTION(pcs) InitializeCriticalSection(pcs)
#define DELETE_CRITICAL_SECTION(pcs) DeleteCriticalSection(pcs)
#define ENTER_CRITICAL_SECTION(pcs) EnterCriticalSection(pcs)
#define LEAVE_CRITICAL_SECTION(pcs) LeaveCriticalSection(pcs)
#endif

static CRITICAL_SECTION cs_mem;

cyg_bool
MEMPOOL_API_CALL MemPool_init(cyg_uint32 size)
{
  void* mem_block;
#ifdef MEMPOOL_WIN
  mem_block = VirtualAlloc(NULL, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
  if (mem_block == NULL)
  {
    return 0;
  }
#endif
#ifdef MEMPOOL_POSIX
  mem_block = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (mem_block == MAP_FAILED)
  {
    return 0;
  }
#endif
  Cyg_dlmalloc_init((cyg_uint8*)mem_block, size);
  INITIALIZE_CRITICAL_SECTION(&cs_mem);
  return 1;
}

void
MEMPOOL_API_CALL MemPool_exit()
{
  struct Cyg_Mempool_Status status;
  Cyg_dlmalloc_get_status(0, &status);
#ifdef MEMPOOL_WIN
  VirtualFree((void*)status.origbase, 0, MEM_RELEASE);
#endif
#ifdef MEMPOOL_POSIX
  munmap((void*)status.origbase, status.origsize);
#endif
  DELETE_CRITICAL_SECTION(&cs_mem);
}

void
MEMPOOL_API_CALL MemPool_get_status(cyg_uint32 flags, struct Cyg_Mempool_Status* status)
{
  memset(status, -1, sizeof(*status));
  ENTER_CRITICAL_SECTION(&cs_mem);
  Cyg_dlmalloc_get_status(flags, status);
  LEAVE_CRITICAL_SECTION(&cs_mem);
}

void*
MEMPOOL_API_CALL MemPool_Memory_Alloc_Func(cyg_uint32 size)
{
  void* new_mem;
  ENTER_CRITICAL_SECTION(&cs_mem);
  new_mem = Cyg_dlmalloc_try_alloc(size);
  LEAVE_CRITICAL_SECTION(&cs_mem);
  return new_mem;
}

void*
MEMPOOL_API_CALL MemPool_Memory_ReAlloc_Func(void* mem, cyg_uint32 new_size)
{
  if (mem == NULL)
  {
    return MemPool_Memory_Alloc_Func(new_size);
  }
  else if (new_size == 0)
  {
    MemPool_Memory_Free_Func(mem);
    return NULL;
  }
  else
  {
    void* new_mem;
    cyg_uint32 old_size;
    ENTER_CRITICAL_SECTION(&cs_mem);
    new_mem = Cyg_dlmalloc_resize_alloc((cyg_uint8*)mem, new_size, &old_size);
    LEAVE_CRITICAL_SECTION(&cs_mem);
    if (new_mem == NULL)
    {
      new_mem = MemPool_Memory_Alloc_Func(new_size);
      if (new_mem != NULL)
      {
        memcpy(new_mem, mem, old_size);
        MemPool_Memory_Free_Func(mem);
      }
    }
    return new_mem;
  }
}

void
MEMPOOL_API_CALL MemPool_Memory_Free_Func(void* mem)
{
  if (mem != NULL)
  {
    ENTER_CRITICAL_SECTION(&cs_mem);
    Cyg_dlmalloc_free((cyg_uint8*)mem);
    LEAVE_CRITICAL_SECTION(&cs_mem);
  }
}
