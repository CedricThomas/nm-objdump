project(nmobjdump)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXE_LINKER_FLAGS "-W -Wall -Wextra")

set(nm_SRC
        nm/include/nm.h
        nm/main.c
        nm/nm.c
        nm/elf.c
        nm/misc.c
        nm/linked_list.c
        nm/extract_symbol.c
        nm/ar.c nm/str.c)

set(objdump_SRC
        objdump/main.c)

add_executable(nm ${nm_SRC})

add_executable(objdump ${objdump_SRC})

target_include_directories(nm PRIVATE nm/include)
target_include_directories(objdump PRIVATE objdump/include)