#include "desctables.hxx"
#include "types.h"
#include "memory.h"
#include "terminal/console.hxx"
#include "memory/kmalloc.hxx"
#define	GDT_ITEMS	5

extern "C" u2 gdt_flush(SGDTPointer *);
/*
class CMemoryHandling
{
};

class CInterruptHandling
{
};

CInterruptHandling* ih;
*/
// Internal function prototypes.
void init_gdt();

SGDTEntry gdt[GDT_ITEMS];
SGDTPointer gdt_ptr;

void init_descriptor_tables()
{
	init_gdt();
}

void init_gdt()
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
	kout->writeValue("Code segment is ");
	kout->writeValue(s);
	kout->writeLine();
}
