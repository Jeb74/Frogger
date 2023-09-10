cmake_minimum_required(VERSION 3.10.2)
project(frogger C)

set(CMAKE_C_STANDARD 11)

# utile per usare valgrind su linux
add_compile_options(-g -Og)

add_executable(frogger
                src/main.c
                src/utils/shortcuts.c
                src/headers/shortcuts.h
                src/headers/structures.h
                src/headers/libraries.h
                src/utils/concurrency/threading.c
                src/headers/threading.h
                src/headers/data.h
                src/game/clock.c
                src/headers/clock.h)

# utile per aggiungere math.h#pow() anche su gcc linux
target_link_libraries(frogger PRIVATE m pthread ncurses)