#pragma once

#include <unordered_map>

// resource_loader uses the following static declarations:
//      - ::type* load( const std::string& )
//      - void unload( ::type*& )
//      - uint64_t size()
//      ::type
template < class resource_loader >
class database
{
private:
    
    using resource_t = typename resource_loader::type;
    
    struct ref_counted
    {
        resource_t* resource;
        uint32_t    counter;
    };
    
    std::unordered_map< resource_t*, std::string > paths;
    std::unordered_map< std::string, ref_counted > resources;
    
public:
    
    resource_t* add( const std::string& path )
    {
        auto it = resources.find( path );
        if ( it != resources.end() ) {
            ++(it->second.counter);
            return it->second.resource;
        } else {
            auto* res = resource_loader::load( path );
            if ( res != nullptr ) {
                resources[ path ] = ref_counted{ res, 1 };
                paths[ res ] = path;
            }
            return res;
        }
    }
    
    void remove( resource_t*& resource )
    {
        auto it = paths.find( resource );
        if ( it != paths.end() ) {
            const auto& it2 = resources[ it.second ];
            if ( --(it2.counter) == 0 ) {
                resource_loader::unload( resource );
                paths.erase( it );
                resources.erase( it2 );
            }
        }
        resource = nullptr;
    }
    
    void clear()
    {
        for ( auto kvp : paths ) {
            resource_loader::unload( kvp.first );
        }
        paths.clear();
        resources.clear();
    }
    
    database() = default;
    ~database() { clear(); }
};
