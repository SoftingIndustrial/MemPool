add_library(mempool STATIC mempool.c dlmalloc.c)
add_executable(testapp testapp.c)
target_link_libraries(testapp PUBLIC mempool)
