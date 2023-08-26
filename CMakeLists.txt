cmake_minimum_required(VERSION 3.10.2)
project(frogger C)

set(CMAKE_C_STANDARD 11)

# utile per usare valgrind su linux
add_compile_options(-g -Og)

add_executable(frogger
                main.c
                utils/shortcuts.c
                utils/shortcuts.h
                utils/structures.h
                utils/libraries.h
                utils/concurrency/threading.c
                utils/concurrency/threading.h
                game/data.h
                game/clock.c
                game/clock.h)

# utile per aggiungere math.h#pow() anche su gcc linux
target_link_libraries(frogger PRIVATE m pthread ncurses)