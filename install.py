#encoding: utf-8

"""
 first argument is the main source file (defaults to main.cpp)
 second argument is "fast" and will make an optimized build (off as default)
"""

compiler            = "clang++"
misc_flags          = [ "-std=c++11", "-pthread", "-fno-rtti", "-fno-exceptions" ]
opt_flags           = [ "-Ofast", "-ffast-math", "-march=native", "-ftree-vectorize", "-flto", "-fassociative-math", "-msse4.2" ]
src_path            = "libsrc"
compile_exts        = [ ".cpp", ".c", ".cc" ] 

import os
import sys

for dir_name in [ "build" ,
                    "res",
                    "res/image",
                    "res/sound"
                    ]:
    path = os.path.join( "./", dir_name )
    if not os.path.isdir( path ):
        os.mkdir( path )
        print( "Created directory '" + dir_name + "'." )


# recursive function that fetches all files in a directory with a given file extension
def get_files( base_path, exts ):
    files = []
    
    if not os.path.exists( base_path ):
        return files
    
    for entry in os.listdir( base_path ):
        path = os.path.join( base_path, entry )
        if os.path.isdir( path ):
            files += get_files( path, exts )
        else:
            for ext in exts:
                if os.path.isfile( path ) and path.endswith( ext ):
                    files += [ path ]
                    break
    
    return files

if __name__ == "__main__":
    # extra arguments
    arg_list = [ arg.lower() for arg in sys.argv[2:] ]
    if not "fast" in arg_list:
        opt_flags = []

    os.chdir( "build" )
    # merge all arguments and pass them to the command line
    files = get_files( os.path.join( "..", src_path ), compile_exts )
    flags = misc_flags + opt_flags
    
    # build object files
    build_cmd = " ".join( [ compiler ] + files + flags + [ "-c" ] )
    
    print( build_cmd )
    os.system( build_cmd )
    
    # build static library
    files = get_files( ".", [ ".o" ] )
    build_cmd = " ".join( [ "ar", "-cvq", "libext.a" ] + files )
    print( build_cmd )
    os.system( build_cmd )
