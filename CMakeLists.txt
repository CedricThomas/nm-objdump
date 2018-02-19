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
        nm/ar.c
        nm/str.c
        nm/print_type.c
        nm/elf/elf_32.c
        nm/elf/elf_64.c)

set(objdump_SRC
        objdump/main.c
        objdump/include/objdump.h objdump/obj.c objdump/elf_format.c objdump/elf.c objdump/ar.c objdump/misc.c objdump/print_section.c objdump/print_hexa.c objdump/include/flags.h objdump/print_header.c objdump/flags.c)

add_executable(nm ${nm_SRC})

add_executable(objdump ${objdump_SRC})

target_include_directories(nm PRIVATE nm/include)
target_include_directories(objdump PRIVATE objdump/include)