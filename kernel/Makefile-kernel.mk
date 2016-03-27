KERNEL_SOURCES := $(shell find kernel -not \( -path kernel/arch -prune \) -name \*.c)
KERNEL_OBJECTS := $(shell echo $(KERNEL_SOURCES:.c=.o) | sed 's/kernel\/kernel/build\/kernel/g')

LIBS = -Lbuild -nostdlib -lk -lc
CPPFLAGS = -D__is_kernel -Ikernel/include

all-kernel:
	$(MAKE) $(KERNEL_ARCH_ASM_OBJECTS)
	$(MAKE) $(KERNEL_OBJECTS)
	$(MAKE) link
	$(MAKE) install-kernel-headers
	$(MAKE) install-kernel

link: build/$(ARCHDIR)/crti.o \
		$(CRTBEGIN_OBJECT) \
		build/$(ARCHDIR)/boot.o \
		build/$(ARCHDIR)/io.o \
		$(KERNEL_ARCH_C_OBJECTS) \
		$(KERNEL_OBJECTS) \
		$(CRTEND_OBJECT) \
		build/$(ARCHDIR)/crtn.o
	$(LD) -o build/kernel.elf $^ $(LDFLAGS) $(LIBS)

# Confirmed working
$(KERNEL_OBJECTS): build%.o: kernel%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(KERNEL_ARCH_ASM_OBJECTS): build%.o: kernel%.asm
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_ARCH_C_OBJECTS): build%.o: kernel%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

install-kernel-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv kernel/include $(DESTDIR)$(INCLUDEDIR)

install-kernel-binary:
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp build/kernel.elf $(DESTDIR)$(BOOTDIR)

install-kernel: install-kernel-headers install-kernel-binary

clean-kernel:
	rm -fv $(KERNEL_OBJECTS) $(KERNEL_ARCH_ASM_OBJECTS) $(KERNEL_ARCH_C_OBJECTS) \
		sysroot/boot/kernel.elf build/kernel.elf
