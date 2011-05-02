LD  = @ld
CC  = @gcc
ASM = @nasm
MOUNT = @mount -o loop floppy.img /mnt
.PHONY: run-shell clean-shell clean-kernel
all: kernel shell.elf
# Clean all
clean: clean-kernel clean-shell

#kernel
KERNEL_DIR    = core
KRN_LD_FLAGS  = -T$(KERNEL_DIR)/kernel.ld
KRN_CC_FLAGS  = -g -DDEBUG=0 -DPOST=0 -m32 -Wno-overflow -Wall -Werror
KRN_ASM_FLAGS = -f elf -o

KRN_OBJ_S     = $(KERNEL_DIR)/int.o $(KERNEL_DIR)/start.o 
KRN_SRC_S     = $(KERNEL_DIR)/int.S $(KERNEL_DIR)/start.S
KRN_OBJ_STA_S     = $(KERNEL_DIR)/start.o 
KRN_SRC_STA_S     = $(KERNEL_DIR)/start.S
KRN_OBJ_INT_S     = $(KERNEL_DIR)/int.o 
KRN_SRC_INT_S     = $(KERNEL_DIR)/int.S 

KRN_OBJ_C     = $(KERNEL_DIR)/init.o
KRN_SRC_C     = $(KERNEL_DIR)/init.c
KRN_SRC_C_UNC = $(KERNEL_DIR)/*.c $(KERNEL_DIR)/*.h


kernel: kernel-asm $(KRN_OBJ_C) 
	@echo "  LD     kernel"
	$(LD) $(KRN_LD_FLAGS) -o $@ $(KRN_OBJ_S) $(KRN_OBJ_C) 
        
$(KRN_OBJ_C): $(KRN_SRC_C) $(KRN_SRC_C_UNC)
	@echo "  CC     kernel"
	$(CC) $(KRN_CC_FLAGS) -c -o $@ $(KRN_SRC_C)
    
kernel-asm: $(KRN_SRC_S)
	@echo "  ASM    kernel"
	$(ASM) $(KRN_ASM_FLAGS) $(KERNEL_DIR)/start.o $(KERNEL_DIR)/start.S
	$(ASM) $(KRN_ASM_FLAGS) $(KERNEL_DIR)/int.o   $(KERNEL_DIR)/int.S


# Clean kernel
clean-kernel:
	@echo "  CLEAN  kernel"
	@rm kernel
	@rm core/init.o
	@rm core/int.o
	@rm core/start.o
#US-Mode
STDARG_PATH  = /usr/lib/gcc/i586-suse-linux/4.5/include/stdarg.h
US_CC_FLAGS  = -include $(STDARG_PATH) -g -m32 -Wall -Iinclude
US_LD_FLAGS  = -melf_i386 -Tinclude/app.ld
US_SRC_C_UNC = include/*.h include/*.c
# Compile App: shell
shell.elf: include/start.o shell/shell.o
	@echo "  LD     shell"
	$(LD) $(US_LD_FLAGS) -o $@ $^
	@echo "  MOUNT  shell"
	$(MOUNT)
	@cp shell.elf /mnt
	@umount /mnt

shell/shell.o: $(US_SRC_C_UNC) shell/main.c shell/*.c
	@echo "  CC     shell"
	$(CC) $(US_CC_FLAGS) -c -o $@ shell/main.c
include/start.o: include/start.S
	@echo "  ASM    USMode"
	nasm -f elf -o include/start.o include/start.S


# Run App shell
run-shell:
	@echo "  DEBUG  shell"
	qemu -soundhw pcspk -s -S -m 526 -kernel kernel -initrd shell.elf -fda floppy.img -append login&
	gdb -symbol=kernel --command='target remote localhost:1234'
# Clean shell
clean-shell:
	@echo "  CLEAN  shell"
	@rm shell.elf
	@rm shell/shell.o
floppy: 
	@echo "  GRUB   LionOS "
	@dd if=/dev/zero of=lionos.img bs=1024 count=1440
	@mkdosfs -F 12 -I lionos.img
	@mount -o loop lionos.img /mnt
	@mkdir -p /mnt/boot/grub
	@cp /boot/grub/stage1 /mnt/boot/grub/.
	@cp /boot/grub/stage2 /mnt/boot/grub/.
	@cp /boot/grub/stage* /mnt/boot/grub/.
	@cp /boot/grub/fat_stage1_5 /mnt/boot/grub/.
	@cp kernel /mnt
	@cp shell.elf /mnt
	@cp menu.lst /mnt/boot/grub
	@umount /mnt
	@echo -e "device (fd0) lionos.img\nroot (fd0)\nsetup (fd0)\nquit" | grub
  
run:
	@echo "  RUN    LionOS Floppy in QEMU"
	@qemu -soundhw pcspk -m 528 -fda lionos.img
