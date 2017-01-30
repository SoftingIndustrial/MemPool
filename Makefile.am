lib_LIBRARIES = libmempool.a
include_HEADERS = mempool.h dlmalloc.h common.h
check_PROGRAMS = testapp
TESTS = testapp

libmempool_a_SOURCES = mempool.c dlmalloc.c
testapp_SOURCES = testapp.c
testapp_LDADD = libmempool.a
