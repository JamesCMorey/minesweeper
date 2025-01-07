cmake_minimum_required(VERSION 3.15)
project(minesweeper)

add_executable(a.out
    main.c
    stage.c
)
set_target_properties(a.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR})
