#pragma once

void EMPTY_FUNCTION(){}

#define PROGRAM_NAME "aa"
#define MAXIMUM_PERCIEVABLE_FRAMERATE (24)

template <class T>
auto dereference_anonymous_type( T const * const x ) { return *x; }

#define ANONYMOUS_TYPE_FROM_PTR( at_ptr )\
    ( decltype( dereference_anonymous_type( at_ptr ) ) )

