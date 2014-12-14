enable_language(ASM)

set(sources
	boot.S
	memory.S
	port.S
	desctables.S
	main.cxx
	terminal/terminal.cxx
	terminal/console.cxx
	desctables.cxx
)

set(include_dirs
	../include
)

include_directories(${include_dirs})
add_executable(kernel ${sources})
target_link_libraries(kernel ${libraries})
