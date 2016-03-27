BINARIES = build/libc.a build/libk.a build/libg.a

LIBC_SOURCES := $(shell find libc -name **.c)
LIBC_OBJECTS := $(shell echo $(LIBC_SOURCES:.c=.o) | sed 's/libc/build\/libc/g')
LIBK_OBJECTS := $(shell echo $(LIBC_OBJECTS:.o=.libk.o))

CPP_FLAGS = -D__is_libc
LIBK_CFLAGS = $(CFLAGS)
LIBK_CPPFLAGS = $(CPP_FLAGS) -D__is_kernel

LIBK_INCLUDES = $(INCLUDES) -Ikernel/include

all-libc:
	$(MAKE) $(LIBC_OBJECTS)
	$(MAKE) $(LIBK_OBJECTS)
	$(MAKE) $(BINARIES)
	$(MAKE) install-libc

build/libc.a: $(LIBC_OBJECTS) $(LIBC_ARCH_ASM_OBJECTS)
	$(AR) rcs $@ $^

build/libg.a:
	$(AR) rcs $@

build/libk.a: $(LIBK_OBJECTS)
	$(AR) rcs $@ $^

$(LIBC_OBJECTS): build%.o: .%.c
	$(CC) $(INCLUDES) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBK_OBJECTS): build%libk.o: .%c
	$(CC) $(LIBK_INCLUDES) $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -c $< -o $@

install-libc-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv libc/include $(DESTDIR)$(INCLUDEDIR)

install-libc-libs:
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

install-libc: install-libc-headers install-libc-libs

#$(LIBC_ARCH_ASM_OBJECTS): build%.o: libc%.asm
#	nasm -felf32 $< -o $@

clean-libc:
	rm -fv $(LIBC_OBJECTS) $(LIBC_ARCH_ASM_OBJECTS) $(LIBK_OBJECTS) $(BINARIES)
	find sysroot/usr/lib -name '*.a' -type f -delete

