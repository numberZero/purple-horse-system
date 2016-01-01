#pragma once
#include "types.h"

packed_struct SMultibootInfo
{
// comment format: "flags_bit:field_offset. flag_name"
// -:0 <required>
	packed_union
	{
		uint32_t flags;
		packed_struct
		{
			bool memory_info:1; // 0
			bool boot_device:1;
			bool command_line:1;
			bool modules:1;
			
			bool symbols_aout:1; // 4
			bool symbols_elf:1;
			bool memory_map:1;
			bool drive_map:1;
			
			bool config_table:1; // 8
			bool boot_loader_name:1;
			bool apm_table:1;
			bool vbe:1;
		} flag;
	};
	
// 0:4 memory_info
	uint32_t mem_lower;
	uint32_t mem_upper;
	
// 1:12. boot_device
	uint32_t boot_device;
	
// 2:16. command_line
	char* cmdline;
	
// 3:20. modules
	uint32_t mods_count;
	void* mods_addr;
	
// 4,5:28. symbols
	packed_union
	{
		u4 data[4];
		
// 4:28. symbols_aout
		packed_struct
		{
		};
		
// 5:28. symbols_elf
		packed_struct
		{
		};
 	};
	
// 6:44. memory_map
	uint32_t mmap_length;
	void* mmap_addr;
	
// 7:52. drive_map
	uint32_t drives_length;
	void* drives_addr;
	
// 8:60. config_table
	void* config_table;
	
// 9:64. boot_loader_name
	char* boot_loader_name;
	
// 10:68. apm_table
	void* apm_table;
	
// 11:72. vbe
	packed_struct
	{
		uint32_t vbe_control_info;
		uint32_t vbe_mode_info;
		uint16_t vbe_mode;
		uint16_t vbe_interface_seg;
		uint16_t vbe_interface_off;
		uint32_t vbe_interface_len;
	};
};
