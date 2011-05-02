/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   VMM
*/
#ifndef VMM_H
#define VMM_H

#define PTEpRESENT 0x001
#define PTEwRITE   0x002
#define KERNEL_PRV 3
#define USER_PRV 7

#include "../datatypes.h"


pageDir_t kernelPageDir;

void vmmMapPage (pageDir_t context, void *vaddr, void *paddr, uint8_t flags );
void vmmActivateContext( pageDir_t context);
pageDir_t  vmmCreateContext();
void vmmInit();

#include "vmm.c"

#endif
