#pragma once

void EMPTY_FUNCTION(){}

#define PROGRAM_NAME "game"

template <class T>
auto dereference_anonymous_type( T const * const x ) { return *x; }

#define ANONYMOUS_TYPE_FROM_PTR( at_ptr )\
    ( decltype( dereference_anonymous_type( at_ptr ) ) )

