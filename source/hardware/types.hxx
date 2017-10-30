#pragma once
#include "types.h"

packed_struct SCommonRegisters
{
	u4 edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

packed_struct SInterruptRegisters
{
	u4 eip, cs, eflags;
};
