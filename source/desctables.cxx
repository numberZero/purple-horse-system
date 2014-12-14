#include "desctables.h"
#include "types.h"
#define	GDT_ITEMS	5
#define	IDT_ITEMS	256

extern "C" void gdt_flush(SGDTPointer *);

// Internal function prototypes.
static void init_gdt();

SGDTEntry gdt[GDT_ITEMS];
SGDTPointer gdt_ptr;
/*
idt_entry_t idt[256];
idt_ptr_t   idt_ptr;
*/

void init_descriptor_tables()
{
	init_gdt();
}

static void init_gdt()
{
	gdt_ptr.base = gdt;
	gdt_ptr.limit = sizeof(SGDTEntry) * GDT_ITEMS - 1;
	
	gdt[0].Clear();
	gdt[1].Setup(0, 0xFFFFFFFF, 0, (SGDTEntry::EDescriptorType)1, (SGDTEntry::ESegmentType)10, SGDTEntry::GranularityKByte, SGDTEntry::Bits32);
	gdt[2].Setup(0, 0xFFFFFFFF, 0, (SGDTEntry::EDescriptorType)1, (SGDTEntry::ESegmentType)2, SGDTEntry::GranularityKByte, SGDTEntry::Bits32);
	gdt[3].Setup(0, 0xFFFFFFFF, 3, (SGDTEntry::EDescriptorType)1, (SGDTEntry::ESegmentType)10, SGDTEntry::GranularityKByte, SGDTEntry::Bits32);
	gdt[4].Setup(0, 0xFFFFFFFF, 3, (SGDTEntry::EDescriptorType)1, (SGDTEntry::ESegmentType)2, SGDTEntry::GranularityKByte, SGDTEntry::Bits32);

	gdt_flush(&gdt_ptr);
}
