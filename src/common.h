#pragma once

#include <chrono>

void EMPTY_FUNCTION(){}

#define PROGRAM_NAME "aa"
#define MAXIMUM_PERCIEVABLE_FRAMERATE (24)

#define BENCHMARK BENCHMARK_TIME(1e3)

#define BENCHMARK_TIME(t_mult) \
for ( struct {\
    int i = 1;\
    std::chrono::steady_clock::time_point start\
        = std::chrono::high_resolution_clock::now(); } s{};\
        s.i--;\
        printf( "%f\n",\
              ( std::chrono::high_resolution_clock::now()\
                                - s.start ).count() / (1e9*(t_mult)) ) )

#define BENCHMARK_CYCLES \
for ( struct { int i = 1; uint64_t start = __rdtsc(); } s{};\
    s.i--;\
    printf( "%d\n", __rdtsc() - start ) )


template <class T>
auto dereference_anonymous_type( T const * const x ) { return *x; }

#define ANONYMOUS_TYPE_FROM_PTR( at_ptr )\
    ( decltype( dereference_anonymous_type( at_ptr ) ) )

