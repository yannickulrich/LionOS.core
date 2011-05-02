/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   PMM
*/
#ifndef PMM_H
#define PMM_H
#include "../panic.h"
/*
 * Der Einfachheit halber deklarieren wir die maximal benoetige Bitmapgroesse
 * statisch (Wir brauchen 4 GB / 4 kB = 1M Bits; 1M Bits sind 1M/8 = 128k
 * Eintraege fuer das Array)
 *
 * Willkuerliche Festlegung: 1 = Speicher frei, 0 = Speicher belegt
 */
#define BITMAP_SIZE (128 * (1 << 10))
static uint32_t pmmbitmap[BITMAP_SIZE];
extern const void kernel_start;
extern const void kernel_end;
void* pmm_malloc(size_t size);
void free(void* page);
void pmm_mark_used(void* page);
void pmminit(struct multiboot_info *mb_info);
void* pmm_prepare_alloc(size_t num);
void USmalloc(size_t size, void** p);
void* kmemcpy(void* dest, const void* src, size_t count);
void* kmemset(void* dest, char val, size_t count);
#define PAGE_SIZE (1 << 12)
#define PMM_BITS_PER_ELEMENT (8 * sizeof(uint32_t))
#include "pmm.c"
#endif
