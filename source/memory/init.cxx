#include "init.hxx"
#include "misc.h"
#include "terminal/console.hxx"
#include "memory/kmalloc.hxx"

CMemoryInitializationFacility *memory_initialization_facility;

extern "C" u2 gdt_flush(SGDTPointer *);

CMemoryInitializationFacility::CMemoryInitializationFacility()
{
	kout->writeLine("Initializing GDT...");
	gdt_ptr.base = gdt;
	gdt_ptr.limit = sizeof(gdt) - 1;

	gdt[0].Clear();
	gdt[1].Setup(0, 0xFFFFFFFF, RingKernel, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentCode);
	gdt[2].Setup(0, 0xFFFFFFFF, RingKernel, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentData);
	gdt[3].Setup(0, 0xFFFFFFFF, RingUser, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentCode);
	gdt[4].Setup(0, 0xFFFFFFFF, RingUser, SGDTEntry::DescriptorDefault, SGDTEntry::SegmentData);
	kout->writeLine("GDT initialized");

	long s = gdt_flush(&gdt_ptr);
	kout->writeLine("GDT loaded");
	kout->writeLine("Code segment was ", s);
}

SGDTEntry const &CMemoryInitializationFacility::entry(size_t index) const
{
	if(index >= entries)
		die("Invalid GDT entry access");
	return gdt[index];
}
