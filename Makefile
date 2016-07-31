ARCH ?= x86_64
KERNEL := build/kernel-$(ARCH).bin
ISO := build/os-$(ARCH).iso

AS := nasm
ASFLAGS := -felf64
CC := $(ARCH)-elf-gcc --sysroot=../libc/sysroot -isystem=/usr/include
LD := $(ARCH)-elf-ld
CSTD := -std=gnu11
WARNINGS := -Wall -Werror -Wextra
LIBS := -nostdlib -lk
CFLAGS := $(CSTD) -ffreestanding $(LIBS) $(LIBS) $(WARNINGS) 

LINKER_SCRIPT := src/arch/$(ARCH)/linker.ld
GRUB_CFG := src/arch/$(ARCH)/grub/grub.cfg
ASSEMBLY_SOURCES := $(wildcard src/arch/$(ARCH)/*.asm)
ASSEMBLY_OBJECTS := $(patsubst src/arch/$(ARCH)/%.asm, build/arch/$(ARCH)/%.o, $(ASSEMBLY_SOURCES))
C_SOURCES := $(wildcard src/*.c)
C_OBJECTS := $(patsubst src/%.c, build/%.o, $(C_SOURCES))

.PHONY: all clean run iso

all: $(KERNEL)

clean:
	rm -r build

run: $(ISO)
	qemu-system-x86_64 -monitor stdio -cdrom $(ISO)

iso: $(ISO)

$(ISO): $(KERNEL) $(GRUB_CFG)
	mkdir -p build/isofiles/boot/grub
	cp $(KERNEL) build/isofiles/boot/kernel.bin
	cp $(GRUB_CFG) build/isofiles/boot/grub
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $(ISO) build/isofiles
	rm -rfv build/isofiles

$(KERNEL): $(ASSEMBLY_OBJECTS) $(C_OBJECTS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(KERNEL) $(ASSEMBLY_OBJECTS) $(C_OBJECTS)

# compile assembly files
build/arch/$(ARCH)/%.o: src/arch/$(ARCH)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<
