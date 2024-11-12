cmake_minimum_required(VERSION 3.29)
project(Sistema_Emprestimo_Salas C)

set(CMAKE_C_STANDARD 11)

# Find SQLite3 library
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(Sistema_Emprestimo_Salas
    src/database.c
    src/main.c)

target_link_libraries(Sistema_Emprestimo_Salas ${SQLite3_LIBRARIES})