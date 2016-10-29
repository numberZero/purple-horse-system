enable_language(ASM)

set(sources
	boot.S
	memory.S
	port.S
	desctables.S
	main.cxx
	terminal/terminal.cxx
	terminal/console.cxx
	interrupts/handling.cxx
	interrupts/helpers.S
	interrupts/irq.cxx
	memory/kmalloc.cxx
	desctables.cxx
)

set(include_dirs
	.
	../include
)

include_directories(${include_dirs})
add_executable(kernel ${sources})
target_link_libraries(kernel ${libraries})
