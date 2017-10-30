cmake_minimum_required(VERSION 3.0)
project(system)

add_subdirectory(source)

get_filename_component(linkscript "link.ld" ABSOLUTE)

option(X64 "Generate x64 code" OFF)

if(${X64})
	set(MACHINE_FLAGS "-m64")
	set(TARGET_FLAGS "-Wl,-melf_x86_64")
	target_compile_definitions(kernel PUBLIC X86_64)
else()
	set(MACHINE_FLAGS "-m32")
	set(TARGET_FLAGS "-Wl,-melf_i386")
endif()

set_target_properties(kernel PROPERTIES
	COMPILE_FLAGS "${MACHINE_FLAGS} -Wall -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-rtti -fno-exceptions -std=c++11"
	LINK_FLAGS "${MACHINE_FLAGS} ${TARGET_FLAGS} -nostdlib -nostdinc -fno-builtin -fno-stack-protector -T${linkscript}"
	LINK_DEPENDS ${linkscript}
	)
