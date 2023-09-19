cmake_minimum_required(VERSION 3.10.2)
project(frogger C)

set(CMAKE_C_STANDARD 11)

# utile per usare valgrind su linux
add_compile_options(-g -Og)

add_executable(frogger
                src/main.c
                
                src/headers/shortcuts.h
                src/utils/shortcuts.c

                src/headers/structures.h
                src/utils/structures.c

                src/headers/threading.h
                src/utils/concurrency/threading.c

                src/headers/processing.h
                src/utils/concurrency/processing.c

                src/headers/clock.h
                src/game/clock.c

                src/headers/board.h
                src/game/board.c

                src/headers/frog.h
                src/game/frog.c

                src/headers/graphics.h
                src/game/graphics/graphics.c

                src/headers/data.h
                src/utils/data.c)

# utile per aggiungere math.h#pow() anche su gcc linux
target_link_libraries(frogger PRIVATE m pthread ncursesw)