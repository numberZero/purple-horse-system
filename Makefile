SOURCES=boot.S memory.S port.S main.c terminal/terminal.cxx
INCLUDE_DIRS=include
KERNEL=bin/kernel

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector $(INCLUDE_FLAGS) -std=c11
CXXFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector $(INCLUDE_FLAGS) -std=c++11
ASFLAGS=-m32 $(INCLUDE_FLAGS)
LDFLAGS=-melf_i386 -Tlink.ld

INCLUDE_FLAGS=$(INCLUDE_DIRS:%=-I%)
KERNEL_DIR=`dirname $(KERNEL)`
OBJECTS=$(SOURCES:%=build/%.o)

all: $(KERNEL)

clean: autoclean
	rm $(KERNEL)

autoclean:
	rm -r build

$(KERNEL): $(OBJECTS)
	mkdir -p `dirname $@`
	ld $(LDFLAGS) -o $@ $(OBJECTS)

build/%.S.o: source/%.S
	mkdir -p `dirname $@`
	gcc $(ASFLAGS) -c $< -o $@

build/%.c.o: source/%.c
	mkdir -p `dirname $@`
	gcc $(CFLAGS) -c $< -o $@

build/%.cxx.o: source/%.cxx
	mkdir -p `dirname $@`
	g++ ${CXXFLAGS} -c $< -o $@

%.cxx:
