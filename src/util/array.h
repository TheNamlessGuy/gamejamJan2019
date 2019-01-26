#pragma once

#include <stdint.h>

template <class T, uint32_t ALLOC_SIZE>
struct array
{
    T           data[ ALLOC_SIZE ];
    uint32_t    used_size;

    array() { used_size = 0; };

    inline void push_back( const T& elem )  { data[ used_size++ ] = elem; }
    inline void remove( uint32_t index )    { data[index] = data[ --used_size ]; }

    inline uint32_t size()          { return used_size; }
    inline uint32_t maximilian()    { return ALLOC_SIZE; }

    // some C++ algorithms use this
    T* begin()              { return data; }
    T* end()                { return data+used_size; }
};
