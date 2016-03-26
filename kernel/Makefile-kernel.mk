KERNEL_SOURCES := $(shell find kernel -name **.c)
KERNEL_OBJECTS := $(shell echo $(KERNEL_SOURCES:.c=.o) | sed 's/kernel\/kernel/build\/kernel/g')

all-kernel:
	$(MAKE) $(KERNEL_ARCH_ASM_OBJECTS)
	$(MAKE) $(KERNEL_OBJECTS)
	$(MAKE) link
	$(MAKE) install-headers
	$(MAKE) install-kernel

link: $(KERNEL_ARCH_ASM_OBJECTS) $(KERNEL_OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.elf $^

# Confirmed working
$(KERNEL_OBJECTS): build%.o: kernel%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ARCH_ASM_OBJECTS): build%.o: kernel%.asm
	nasm -felf32 $< -o $@

#$(KERNEL_ARCH_C_OBJECTS): build%.o: arch/i386%.c
#	$(CC) $(CFLAGS) -c $< -o $@

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv kernel/include $(DESTDIR)$(INCLUDEDIR)

install-kernel:
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp build/kernel.elf $(DESTDIR)$(BOOTDIR)

clean-kernel:
	rm -fv $(KERNEL_OBJECTS) $(KERNEL_ARCH_ASM_OBJECTS) sysroot/boot/kernel.elf
	find sysroot/usr/include -name '*.h' -type f -delete
