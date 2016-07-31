ARCH ?= x86_64
KERNEL := build/kernel-$(ARCH).bin
ISO := build/os-$(ARCH).iso

AS := nasm
ASFLAGS := -felf64
CC := $(ARCH)-elf-gcc --sysroot=../libc/sysroot -isystem=/usr/include
LD := $(ARCH)-elf-ld
LINKER_SCRIPT := src/arch/$(ARCH)/linker.ld
LDFLAGS := -n -L../libc/sysroot/usr/lib
CSTD := -std=gnu11
WARNINGS := -Wall -Werror -Wextra
INCLUDES := -Iinclude
LIBS := -lk
CFLAGS := $(CSTD) $(INCLUDES) -ffreestanding $(WARNINGS) 

GRUB_CFG := src/arch/$(ARCH)/grub/grub.cfg
ASSEMBLY_SOURCES := $(wildcard src/arch/$(ARCH)/*.asm)
ASSEMBLY_OBJECTS := $(patsubst src/arch/$(ARCH)/%.asm, build/arch/$(ARCH)/%.o, $(ASSEMBLY_SOURCES))
C_SOURCES := $(wildcard src/*.c)
C_ARCH_SOURCES := $(wildcard src/arch/$(ARCH)/*.c)
C_ARCH_OBJECTS := $(patsubst src/%.c, build/%.o, $(C_ARCH_SOURCES))
C_OBJECTS := $(patsubst src/%.c, build/%.o, $(C_SOURCES))
CRTBEGIN_OBJECT := $(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTI_OBJECT := build/arch/x86_64/crti.o
CRTN_OBJECT := build/arch/x86_64/crtn.o
CRT_FILTER := $(CRTI_OBJECT) $(CRTN_OBJECT)
CRTEND_OBJECT := $(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)

OBJECT_LINK_LIST := \
	$(CRTI_OBJECT) \
	$(filter-out $(CRTN_OBJECT), $(filter-out $(CRTI_OBJECT), $(ASSEMBLY_OBJECTS))) \
	$(C_ARCH_OBJECTS) \
	$(C_OBJECTS) \
	$(CRTN_OBJECT)

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

$(KERNEL): $(OBJECT_LINK_LIST) $(LINKER_SCRIPT)
	$(LD) -T $(LINKER_SCRIPT) $(OBJECT_LINK_LIST) -o $(KERNEL) $(LDFLAGS) $(LIBS)

# compile assembly files
build/arch/$(ARCH)/%.o: src/arch/$(ARCH)/%.asm
	mkdir -p $(shell dirname $@)
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LIBS) -c -o $@ $<

print-%  : ; @echo $* = $($*)
