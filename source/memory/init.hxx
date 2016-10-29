#pragma once
#include "types.h"
#include "table.hxx"

class CMemoryInitializationFacility
{
public:
	static constexpr const size_t entries = 5;

	CMemoryInitializationFacility();
	SGDTEntry const &entry(size_t index) const;

private:
	SGDTEntry gdt[entries];
	SGDTPointer gdt_ptr;
};

extern CMemoryInitializationFacility *memory_initialization_facility;
