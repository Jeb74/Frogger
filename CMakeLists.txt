cmake_minimum_required(VERSION 3.10.2)
project(Frogger C)

set(CMAKE_C_STANDARD 11)

# utile per usare valgrind su linux
add_compile_options(-g -Og)

add_executable(Frogger
        main.c
        utils/globalmacros.h
        utils/shortcuts.h
        utils/structures.h
        utils/libraries.h
        chelp/chelp.c
        chelp/chelp.h
        utils/general.h
        files/file_manager.c
        files/file_manager.h
        )

# utile per aggiungere math.h#pow() anche su gcc linux
target_link_libraries(Frogger PRIVATE m)