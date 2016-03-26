LIBC_SOURCES := $(shell find libc -name **.c)
LIBC_OBJECTS := $(shell echo $(LIBC_SOURCES:.c=.o) | sed 's/libc/build\/libc/g')

INCLUDES = -Ilibc/include -Ikernel/include

all-libc:
	$(MAKE) $(LIBC_OBJECTS)

$(LIBC_OBJECTS): build%.o: .%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

#$(LIBC_ARCH_ASM_OBJECTS): build%.o: libc%.asm
#	nasm -felf32 $< -o $@

clean-libc:
	rm -fv $(LIBC_OBJECTS) $(LIBC_ARCH_ASM_OBJECTS)
