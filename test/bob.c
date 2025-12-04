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
    
   
    nob_cmd_append(&cmd, "gcc", "-g", "-O0", "-Wall", "-Wextra", "-o", "build/test", "src/main.c", "-I./include", 
                   "-I/usr/include/freetype2", "-L./build/", "-lglfw", "-lm", "-ldl", "-lfreetype", "-lhaykal");

#else
    nob_cmd_append(&cmd, "cl", "-I.", "-o", BUILD_FOLDER"hello", SRC_FOLDER"hello.c");
#endif // _MSC_VER

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}


