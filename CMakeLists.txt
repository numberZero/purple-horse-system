cmake_minimum_required(VERSION 3.14)
project(system)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(source)

get_filename_component(linkscript "link.ld" ABSOLUTE)

set_target_properties(kernel PROPERTIES
	COMPILE_FLAGS "-m32 -mgeneral-regs-only -Wall -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-rtti -fno-exceptions"
	LINK_FLAGS "-m32 -Wl,-melf_i386 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -T${linkscript}"
	LINK_DEPENDS ${linkscript}
	)
