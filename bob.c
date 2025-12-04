// bob the builder



#define NOB_IMPLEMENTATION

#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

#if !defined(_MSC_VER)
    
   
    nob_cmd_append(&cmd, "gcc", "-fPIC", "-shared", "-Wall", "-Wextra", "-o", "build/libhaykal.so", "src/haykal.c", "-I./include", "-I/usr/include/freetype2", "-lglfw", "-lm", "-ldl", "-lfreetype");

#else
    // On MSVC
    nob_cmd_append(&cmd, "cl", "-I.", "-o", BUILD_FOLDER"hello", SRC_FOLDER"hello.c");
#endif // _MSC_VER

    if (!nob_cmd_run(&cmd)) return 1;


    return 0;
}


