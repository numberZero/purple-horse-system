cmake_minimum_required(VERSION 3.0)
project(system)

add_subdirectory(source)

get_filename_component(linkopt "link.ld" ABSOLUTE)

#set(CMAKE_C_COMPILE_FLAGS -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=c11)
#set(CMAKE_CXX_COMPILE_FLAGS -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -std=c++11)
#set(CMAKE_ASM_COMPILE_FLAGS -m32)
#set(CMAKE_LINKER_FLAGS -melf_i386 -Tlink.ld)

set_target_properties(kernel PROPERTIES
	COMPILE_FLAGS "-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-rtti -std=c++11"
	LINK_FLAGS "-m32 -Wl,-melf_i386 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -T${linkopt}")
