enable_language(ASM)

set(sources
	common/memory.S
	hardware/boot.S
	hardware/port.S
	interrupts/handling.cxx
	interrupts/helpers.S
	interrupts/irq.cxx
	main.cxx
	memory/helpers.S
	memory/init.cxx
	memory/kmalloc.cxx
	terminal/terminal.cxx
	terminal/console.cxx
)

set(include_dirs
	.
	./common
	../include
)

include_directories(${include_dirs})
add_executable(kernel ${sources})
target_link_libraries(kernel ${libraries})
