cmake_minimum_required(VERSION 3.27)
project(Magica C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -Wuninitialized -pedantic -g)

add_executable(magica
        data.c
        main.c
        battle.c
        print.c
        save_load.c
)
target_include_directories(magica PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(magica -lncurses -lm)





