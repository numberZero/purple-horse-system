#include "desctables.h"
#include "types.h"
#include "memory.h"
#include "port.h"
#include "terminal/console.hxx"
#define	GDT_ITEMS	5
#define	IDT_ITEMS	256

using interrupt_handler_t = void *;

typedef struct registers
{
	u4 ds;                  // Data segment selector
	u4 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	u4 int_no, err_code;    // Interrupt number and error code (if applicable)
	u4 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

extern "C"
{
	u2 gdt_flush(SGDTPointer *);
	void idt_flush(SIDTPointer *);
	void interrupt_handler(registers_t regs);
	void irq_handler(registers_t regs);
	
	extern interrupt_handler_t interrupt_handler_0, interrupt_handler_1, interrupt_handler_2, interrupt_handler_3, interrupt_handler_4, interrupt_handler_5, interrupt_handler_6, interrupt_handler_7;
	extern interrupt_handler_t interrupt_handler_8, interrupt_handler_9, interrupt_handler_10, interrupt_handler_11, interrupt_handler_12, interrupt_handler_13, interrupt_handler_14, interrupt_handler_15;
	extern interrupt_handler_t interrupt_handler_16, interrupt_handler_17, interrupt_handler_18, interrupt_handler_19, interrupt_handler_20, interrupt_handler_21, interrupt_handler_22, interrupt_handler_23;
	extern interrupt_handler_t interrupt_handler_24, interrupt_handler_25, interrupt_handler_26, interrupt_handler_27, interrupt_handler_28, interrupt_handler_29, interrupt_handler_30, interrupt_handler_31;
	extern interrupt_handler_t irq_handler_0, irq_handler_1, irq_handler_2, irq_handler_3, irq_handler_4, irq_handler_5, irq_handler_6, irq_handler_7;
	extern interrupt_handler_t irq_handler_8, irq_handler_9, irq_handler_10, irq_handler_11, irq_handler_12, irq_handler_13, irq_handler_14;
}

// Internal function prototypes.
static void init_gdt();
static void init_idt();
static void initialize_pic();

SGDTEntry gdt[GDT_ITEMS];
SGDTPointer gdt_ptr;
SIDTEntry idt[IDT_ITEMS];
SIDTPointer idt_ptr;

void init_descriptor_tables()
{
	init_gdt();
	init_idt();
}

static void init_gdt()
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

static void init_idt()
{
	kout->writeLine("Initializing IDT...");
	idt_ptr.limit = sizeof(idt) - 1;
	idt_ptr.base = idt;

	memset(&idt, 0, sizeof(idt));
// CPU exception handlers
	idt[0].Setup(&interrupt_handler_0, 0x0008, RingKernel);
	idt[1].Setup(&interrupt_handler_1, 0x0008, RingKernel);
	idt[2].Setup(&interrupt_handler_2, 0x0008, RingKernel);
	idt[3].Setup(&interrupt_handler_3, 0x0008, RingKernel);
	idt[4].Setup(&interrupt_handler_4, 0x0008, RingKernel);
	idt[5].Setup(&interrupt_handler_5, 0x0008, RingKernel);
	idt[6].Setup(&interrupt_handler_6, 0x0008, RingKernel);
	idt[7].Setup(&interrupt_handler_7, 0x0008, RingKernel);
	idt[8].Setup(&interrupt_handler_8, 0x0008, RingKernel);
	idt[9].Setup(&interrupt_handler_9, 0x0008, RingKernel);
	idt[10].Setup(&interrupt_handler_10, 0x0008, RingKernel);
	idt[11].Setup(&interrupt_handler_11, 0x0008, RingKernel);
	idt[12].Setup(&interrupt_handler_12, 0x0008, RingKernel);
	idt[13].Setup(&interrupt_handler_13, 0x0008, RingKernel);
	idt[14].Setup(&interrupt_handler_14, 0x0008, RingKernel);
	idt[15].Setup(&interrupt_handler_15, 0x0008, RingKernel);
	idt[16].Setup(&interrupt_handler_16, 0x0008, RingKernel);
	idt[17].Setup(&interrupt_handler_17, 0x0008, RingKernel);
	idt[18].Setup(&interrupt_handler_18, 0x0008, RingKernel);
	idt[19].Setup(&interrupt_handler_19, 0x0008, RingKernel);
	idt[20].Setup(&interrupt_handler_20, 0x0008, RingKernel);
	idt[21].Setup(&interrupt_handler_21, 0x0008, RingKernel);
	idt[22].Setup(&interrupt_handler_22, 0x0008, RingKernel);
	idt[23].Setup(&interrupt_handler_23, 0x0008, RingKernel);
	idt[24].Setup(&interrupt_handler_24, 0x0008, RingKernel);
	idt[25].Setup(&interrupt_handler_25, 0x0008, RingKernel);
	idt[26].Setup(&interrupt_handler_26, 0x0008, RingKernel);
	idt[27].Setup(&interrupt_handler_27, 0x0008, RingKernel);
	idt[28].Setup(&interrupt_handler_28, 0x0008, RingKernel);
	idt[29].Setup(&interrupt_handler_29, 0x0008, RingKernel);
	idt[30].Setup(&interrupt_handler_30, 0x0008, RingKernel);
	idt[31].Setup(&interrupt_handler_31, 0x0008, RingKernel);
// IRQ handlers
	idt[32].Setup(&irq_handler_0, 0x0008, RingKernel);
	idt[33].Setup(&irq_handler_1, 0x0008, RingKernel);
	idt[34].Setup(&irq_handler_2, 0x0008, RingKernel);
	idt[35].Setup(&irq_handler_3, 0x0008, RingKernel);
	idt[36].Setup(&irq_handler_4, 0x0008, RingKernel);
	idt[37].Setup(&irq_handler_5, 0x0008, RingKernel);
	idt[38].Setup(&irq_handler_6, 0x0008, RingKernel);
	idt[39].Setup(&irq_handler_7, 0x0008, RingKernel);
	idt[40].Setup(&irq_handler_8, 0x0008, RingKernel);
	idt[41].Setup(&irq_handler_9, 0x0008, RingKernel);
	idt[42].Setup(&irq_handler_10, 0x0008, RingKernel);
	idt[43].Setup(&irq_handler_11, 0x0008, RingKernel);
	idt[44].Setup(&irq_handler_12, 0x0008, RingKernel);
	idt[45].Setup(&irq_handler_13, 0x0008, RingKernel);
	idt[46].Setup(&irq_handler_14, 0x0008, RingKernel);
	kout->writeLine("IDT initialized");
	
	initialize_pic();
	
	idt_flush(&idt_ptr);
	kout->writeLine("IDT loaded");
	outb(0x43, 0b00110010);
	outb(0x40, 0);
	outb(0x40, 0);
	u2 divisor = 1193180 / 19;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	u1 l = (u1)(divisor & 0xFF);
	u1 h = (u1)( (divisor>>8) & 0xFF );

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
	kout->writeLine("Timer started");
}

static void initialize_pic()
{
	kout->writeLine("Initializing PIC...");
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	kout->writeLine("PIC initialized");
}

void interrupt_handler(registers_t regs)
{
	kout->writeLine("Interrupt ", regs.int_no, " received");
}

void irq_handler(registers_t regs)
{
	kout->writeLine("IRQ ", regs.err_code, " interrupt ", regs.int_no, " received");
	if(regs.err_code == 1)
	{
		u1 v = inb(0x60);
		kout->writeLine("Keyboard input: ", v);
		if(v == 1) // escape
		{
			kout->setStyle({LightRed});
			kout->writeLine("Keyboard halt");
			die("System halted by user");
		}
	}
	if(regs.int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
}
