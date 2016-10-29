#pragma once
#include "types.h"

class KAllocator
{
	uintptr_t base_address;

public:
	KAllocator(void* base) :
		base_address(reinterpret_cast<uintptr_t>(base))
	{
	}

	void* alloc(size_t size, size_t align_bits = 0)
	{
		if(align_bits > 16) // > 64 KiB align
			return nullptr;
		uintptr_t mask = 1;
		uintptr_t addr;
		mask <<= align_bits;
		mask -= 1;
		addr = base_address;
		addr += mask;
		addr &= ~mask;
		base_address = addr + size;
		return reinterpret_cast<void*>(addr);
	}

	static KAllocator* allocator;
};

class undeletable_t
{
};

static undeletable_t undeletable;

inline void* operator new(size_t size, undeletable_t = undeletable)
{
	return KAllocator::allocator->alloc(size);
}

inline void operator delete(void*, undeletable_t = undeletable)
{
}
