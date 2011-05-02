#!/bin/bash
if [ "$1" == "-help" ]
then
	echo "  ARG    Help"
    echo "Makefile Updater"
    echo "$0 [-debug | -dbg [-p] | -autorun APPNAME | -autofloppy] "
    exit
fi

if [ "$1" == "-debug" ]
then
	echo "  ARG    GDB-Debug"
 	DEBUG="1"
else DEBUG="0"
fi
if [ "$2" == "-p" ]
then
	echo "  ARG    Print Kernel Post Commands"
 	POST="1"
else POST="0"
fi
if [ "$1" == "-dbg" ]
then
	echo "  ARG    Kernel Debug Mode"
 	DG="1"
else DG="0"
fi

if [ "$1" == "-autorun" ]
then
	echo "  ARG    Autorun $2"
fi 


echo "  GEN    General Arguments"

if [ "$(uname)" == "Darwin" ]
then
    IMAGE="floppy.img"
    echo "LD    = i386-elf-ld" > Makefile
    echo "CC    = i386-elf-gcc-4.3.2" >> Makefile
    echo "ASM   = nasm" >> Makefile
    #echo "MOUNT = hdiutil mount -mountpoint /mnt $IMAGE" >> Makefile
    echo "MOUNT = " >> Makefile
    
else
    IMAGE="floppy.img"
    echo "LD  = @ld" > Makefile
    echo "CC  = @gcc" >> Makefile
    echo "ASM = @nasm" >> Makefile
    echo "MOUNT = @mount -o loop $IMAGE /mnt" >> Makefile
fi
printf ".PHONY:" >> Makefile
for i in $(ls); do
    
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
                printf " run-$i" >> Makefile
                printf " clean-$i" >> Makefile
            fi
        fi
    fi
done
printf " clean-kernel" >> Makefile
echo "" >> Makefile
printf "all: kernel" >> Makefile
for i in $(ls); do
    
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
                printf " $i.elf" >> Makefile
            fi
        fi
    fi
done
if [ "$1" == "-autorun" ]
then
	printf " run-$2" >> Makefile
fi 
printf "\n" >> Makefile
echo "# Clean all" >> Makefile
printf "clean: clean-kernel" >> Makefile
for i in $(ls); do
    
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
		
                printf " clean-$i" >> Makefile
            fi
        fi
    fi
done

printf "\n" >> Makefile

echo "  GEN    kernel compilation unit"
echo "" >> Makefile
echo "#kernel" >> Makefile
echo "KERNEL_DIR    = core" >> Makefile
echo "KRN_LD_FLAGS  = -T\$(KERNEL_DIR)/kernel.ld" >> Makefile
echo "KRN_CC_FLAGS  = -g -DDEBUG=$DG -DPOST=$POST -m32 -Wno-overflow -Wall -Werror" >> Makefile
echo "KRN_ASM_FLAGS = -f elf -o" >> Makefile
echo "" >> Makefile
echo "KRN_OBJ_S     = \$(KERNEL_DIR)/int.o \$(KERNEL_DIR)/start.o " >> Makefile
echo "KRN_SRC_S     = \$(KERNEL_DIR)/int.S \$(KERNEL_DIR)/start.S" >> Makefile
echo "KRN_OBJ_STA_S     = \$(KERNEL_DIR)/start.o " >> Makefile
echo "KRN_SRC_STA_S     = \$(KERNEL_DIR)/start.S" >> Makefile
echo "KRN_OBJ_INT_S     = \$(KERNEL_DIR)/int.o " >> Makefile
echo "KRN_SRC_INT_S     = \$(KERNEL_DIR)/int.S " >> Makefile
echo "" >> Makefile
echo "KRN_OBJ_C     = \$(KERNEL_DIR)/init.o" >> Makefile
echo "KRN_SRC_C     = \$(KERNEL_DIR)/init.c" >> Makefile
echo "KRN_SRC_C_UNC = \$(KERNEL_DIR)/*.c \$(KERNEL_DIR)/*.h" >> Makefile
echo "" >> Makefile
echo "" >> Makefile
echo "kernel: kernel-asm \$(KRN_OBJ_C) " >> Makefile
echo "	@echo \"  LD     kernel\"" >> Makefile
echo "	\$(LD) \$(KRN_LD_FLAGS) -o \$@ \$(KRN_OBJ_S) \$(KRN_OBJ_C) " >> Makefile
echo "        " >> Makefile
echo "\$(KRN_OBJ_C): \$(KRN_SRC_C) \$(KRN_SRC_C_UNC)" >> Makefile
echo "	@echo \"  CC     kernel\"" >> Makefile
echo "	\$(CC) \$(KRN_CC_FLAGS) -c -o \$@ \$(KRN_SRC_C)" >> Makefile
echo "    " >> Makefile
echo "kernel-asm: \$(KRN_SRC_S)" >> Makefile
echo "	@echo \"  ASM    kernel\"" >> Makefile
echo "	\$(ASM) \$(KRN_ASM_FLAGS) \$(KERNEL_DIR)/start.o \$(KERNEL_DIR)/start.S" >> Makefile
echo "	\$(ASM) \$(KRN_ASM_FLAGS) \$(KERNEL_DIR)/int.o   \$(KERNEL_DIR)/int.S" >> Makefile
echo "" >> Makefile
echo "" >> Makefile

echo "  GEN    kernel clean unit"
echo "# Clean kernel" >> Makefile
echo "clean-kernel:" >> Makefile
echo "	@echo \"  CLEAN  kernel\"" >> Makefile
echo "	@rm kernel" >> Makefile
echo "	@rm core/init.o" >> Makefile
echo "	@rm core/int.o" >> Makefile
echo "	@rm core/start.o" >> Makefile

echo "#US-Mode" >> Makefile

if [ "$(uname)" == "Darwin" ]
then echo "STDARG_PATH  = /usr/lib/gcc/i686-apple-darwin10/4.0.1/include/stdarg.h" >> Makefile
else echo "STDARG_PATH  = /usr/lib/gcc/i586-suse-linux/4.5/include/stdarg.h" >> Makefile
fi
echo "US_CC_FLAGS  = -include \$(STDARG_PATH) -g -m32 -Wall -Iinclude" >> Makefile
echo "US_LD_FLAGS  = -melf_i386 -Tinclude/app.ld" >> Makefile
echo "US_SRC_C_UNC = include/*.h include/*.c" >> Makefile

for i in $(ls); do
    
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
				echo "  GEN    $i compilation unit"
                
                echo "# Compile App: $i" >> Makefile
                echo "$i.elf: include/start.o $i/$i.o" >> Makefile
				echo "	@echo \"  LD     $i\"" >> Makefile
                echo "	\$(LD) \$(US_LD_FLAGS) -o \$@ \$^" >> Makefile
				echo "	@echo \"  MOUNT  $i\"" >> Makefile
                echo "	\$(MOUNT)" >> Makefile
                echo "	@cp $i.elf /mnt" >> Makefile
                echo "	@umount /mnt" >> Makefile
                echo "" >> Makefile
                echo "$i/$i.o: \$(US_SRC_C_UNC) $i/main.c $i/*.c" >> Makefile
				echo "	@echo \"  CC     $i\"" >> Makefile
                echo "	\$(CC) \$(US_CC_FLAGS) -c -o \$@ $i/main.c" >> Makefile
                
            fi
        fi
    fi
    
done

echo "include/start.o: include/start.S" >> Makefile
echo "	@echo \"  ASM    USMode\"" >> Makefile
echo "	nasm -f elf -o include/start.o include/start.S" >> Makefile
echo "" >> Makefile
echo "" >> Makefile


for i in $(ls); do
    
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
				echo "  GEN    $i run unit"
                echo "# Run App $i" >> Makefile
                echo "run-$i:" >> Makefile
                if [ "$DEBUG" == "1" ]
                then 
					echo "	@echo \"  DEBUG  $i\"" >> Makefile
                    echo "	qemu -soundhw pcspk -s -S -m 526 -kernel kernel -initrd $i.elf -fda $IMAGE -append login&" >> Makefile
				    echo "	gdb -symbol=kernel --command='target remote localhost:1234'" >> Makefile
                else
                	echo "	@echo \"  RUN    $i\"" >> Makefile
                    echo "	@qemu -soundhw pcspk -m 526 -kernel kernel -initrd $i.elf -fda $IMAGE -append login" >> Makefile
                fi
				echo "  GEN    $i clean unit"
                echo "# Clean $i" >> Makefile
                echo "clean-$i:" >> Makefile
				echo "	@echo \"  CLEAN  $i\"" >> Makefile
                echo "	@rm $i.elf" >> Makefile
                echo "	@rm $i/$i.o" >> Makefile
            fi
        fi
    fi
    
done


echo "floppy: " >> Makefile
echo "	@echo \"  GRUB   LionOS \"" >> Makefile
echo "	@dd if=/dev/zero of=lionos.img bs=1024 count=1440" >> Makefile
echo "	@mkdosfs -F 12 -I lionos.img" >> Makefile
echo "	@mount -o loop lionos.img /mnt" >> Makefile
echo "	@mkdir -p /mnt/boot/grub" >> Makefile
echo "	@cp /boot/grub/stage1 /mnt/boot/grub/." >> Makefile
echo "	@cp /boot/grub/stage2 /mnt/boot/grub/." >> Makefile
echo "	@cp /boot/grub/stage* /mnt/boot/grub/." >> Makefile
echo "	@cp /boot/grub/fat_stage1_5 /mnt/boot/grub/." >> Makefile
#echo "	@cp /boot/message /mnt/boot/." >> Makefile
echo "	@cp kernel /mnt" >> Makefile
echo "default 0" > menu.lst
echo "timeout 8" >> menu.lst
#echo "gfxmenu /boot/message" >> menu.lst
for i in $(ls); do
    if [ -d $i ]
    then
        if [ "$i" != "include" ]
        then
            if [ "$i" != "core" ]
            then
            	echo "title $i" >> menu.lst
            	echo "    root (fd0)" >> menu.lst
            	echo "    kernel /kernel" >> menu.lst
            	echo "    module /$i.elf" >> menu.lst
            	echo "    boot" >> menu.lst
                echo "	@cp $i.elf /mnt" >> Makefile
            fi
        fi
    fi 
done
echo "	@cp menu.lst /mnt/boot/grub" >> Makefile
echo "	@umount /mnt" >> Makefile
echo "	@echo -e \"device (fd0) lionos.img\nroot (fd0)\nsetup (fd0)\nquit\" | grub" >> Makefile
echo "  " >> Makefile
echo "run:" >> Makefile
echo "	@echo \"  RUN    LionOS Floppy in QEMU\"" >> Makefile
echo "	@qemu -soundhw pcspk -m 528 -fda lionos.img" >> Makefile

