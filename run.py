#encoding: utf-8

"""
 first argument is the main source file (defaults to main.cpp)
 second argument is "fast" and will make an optimized build (off as default)
"""

compiler            = "clang++"
misc_flags          = [ "-std=c++14", "-pthread", "-lm", "-fno-rtti", "-fno-exceptions", "-I../src" ]
libs                = [ "-lSDL2", "-lSDL2_net", "-lSDL2_image", "-lSDL2_ttf", "-lSDL2_mixer", "libext.a" ]
opt_flags           = [ "-Ofast", "-ffast-math", "-march=native", "-ftree-vectorize", "-flto", "-fassociative-math", "-msse4.2" ]
src_path            = "src"
compile_exts        = [ "cpp", "c", "cc" ] 

import os
import sys
import shutil

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
    # find main file
    try:
        main_file = sys.argv[1]
        ext_index = main_file.find(".")
        if ext_index == -1:
            ext_index = len( main_file )
        program_name = main_file[ :ext_index ]
    except:
        main_file       = "main.cpp"
        program_name    = "app"
    program_name += ".exe"
    # extra arguments
    arg_list = [ arg.lower() for arg in sys.argv[ 2: ] ]
    if not "fast" in arg_list:
        opt_flags = []

    os.chdir( "build" )
    print( os.listdir() )
    # merge all arguments and pass them to the command line
    files = [ os.path.join( "..", main_file ) ] + get_files( os.path.join( "..", src_path ), compile_exts )
    flags = misc_flags + libs + opt_flags
    
    # build
    build_cmd = " ".join( [ compiler ] + files + flags + [ "-o", program_name ] )    
    print( build_cmd )
    if ( os.system( build_cmd ) != 0 ):
        sys.exit()
    
    # move/overwrite executable run
    os.system( "mv -f " + program_name + " .." )
    os.chdir( ".." )
    os.system( "./" + program_name )

