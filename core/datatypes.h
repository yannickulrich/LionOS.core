/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                Datatypes
*/


/**
 * Define boolean types.
 * bool or boolean --> char
 * true or TRUE --> 1
 * false or FALSE --> 0
 */
#ifndef Core_Bool
#define Core_Bool
#define bool char
#define boolean bool
#define TRUE 1
#define true TRUE
#define FALSE 0
#define false FALSE
#endif



#ifndef datatyp_H
#define datatyp_H
#define ULONG unsigned long

// Defa‎ult implemention for printf
#define PRINTF_STUB va_list ap; \
	int i = 0; \
	va_start(ap, args);  \
	int d; \
	char c, *s; \
	while (args[i]) \
	{ \
		switch (args[i]) \
		{ \
			case '%': \
				i++; \
				switch(args[i]) \
				{ \
					case 'd':  \
						d = va_arg(ap, int); \
						if (d < 0) { printChar('-'); } \
						pn(((d) >= (0)) ? (d) : (-d)); \
						break; \
					case 'c':  \
						c = (char) va_arg(ap, int); \
						printChar(c); \
						break; \
					case 's':  \
						s =  va_arg(ap, char *); \
						kprintf(s); \
						break; \
					default: printChar('%'); \
				} \
				break; \
			default:  \
				printChar(args[i]); \
				break; \
		} \
		i++; \
	} \
	va_end(ap); ___sysIdle
	




typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;
typedef unsigned int size_t;
typedef unsigned char byte;
typedef unsigned int WORD;
typedef unsigned long DWORD;


//Paging
typedef unsigned long* pageDir_t;
typedef unsigned long* pageTable_t;
// /Paging

#define null (void*) 0
#define NIL null
//#define NULL null

typedef struct {
	uint16_t lsb_handler;
	uint16_t selector;
	uint8_t reserved;
	uint8_t access;
	uint16_t msb_handler;
} gate_descriptor;
/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};



struct multiboot_info {
	uint32_t	mb_flags;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_MEMORY. */
	uint32_t	mb_mem_lower;
	uint32_t	mb_mem_upper;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_BOOT_DEVICE. */
	uint8_t	mb_boot_device_part3;
	uint8_t	mb_boot_device_part2;
	uint8_t	mb_boot_device_part1;
	uint8_t	mb_boot_device_drive;
	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_CMDLINE. */
	char *	mb_cmdline;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_MODS. */
	uint32_t	mb_mods_count;
	struct multiboot_module * mb_mods_addr;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_{AOUT,ELF}_SYMS. */
	uint32_t	mb_elfshdr_num;
	uint32_t	mb_elfshdr_size;
	void *	mb_elfshdr_addr;
	uint32_t	mb_elfshdr_shndx;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_MMAP. */
	uint32_t	mb_mmap_length;
	void *	mb_mmap_addr;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_DRIVES. */
	uint32_t	mb_drives_length;
	void *	mb_drives_addr;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_CONFIG_TABLE. */
	void *	unused_mb_config_table;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_LOADER_NAME. */
	char *	mb_loader_name;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_APM. */
	void *	unused_mb_apm_table;

	/* Valid if mb_flags sets MULTIBOOT_INFO_HAS_VBE. */
	void *	unused_mb_vbe_control_info;
	void *	unused_mb_vbe_mode_info;
	void *	unused_mb_vbe_interface_seg;
	void *	unused_mb_vbe_interface_off;
	uint32_t	unused_mb_vbe_interface_len;
};
typedef struct regs cpu_state;

struct multiboot_mmap {
	uint32_t	size;
	uint64_t	base;
	uint64_t	length;
	uint32_t	type;
};


typedef struct task {
	int writing;
	int priority;
	int pid;
	cpu_state * esp;
	unsigned long kStack;
	int killNext;
	pageDir_t context;
	char *cmdline;
	struct task * next;
} task;

struct multiboot_module {
	void * start;
	void * end;
	char* cmdline;
	uint32_t reserved;
};


typedef struct file_t {
	int ID;
	char name[8];
	uint16_t clusterID;
	char extension[3];
	int hidden;
	int isDir;
	int readOnly;
	size_t size;
	
	struct file_t *next;
} file;

typedef struct directory_t {
	int ID;
	char *name;
	uint16_t clusterID;
	file *first;
	struct directory_t *firstSub;
	
	struct directory_t *next;
} directory;

typedef struct {
	bool free;
	bool defect;
	bool lastCluster;
	int content;
} FATENTRY;


typedef struct {
	int s;
	int min;
	int h;
	int d;
	int m;
	int y;
} timestamp;
#define TimeStamp timestamp
#endif
