include kernel/Makefile-kernel.mk \
		libc/Makefile-libc.mk

.DEFAULT_GOAL = all

# ARCHITECTURE SPECIFIC MAKE INCLUDE
ifeq ($(arch), i386)
	include kernel/arch/i386/make.config \
			libc/arch/i386/make.config
ifeq ($(arch), )
	$(error "Please specify architecture.")
endif
endif

CSTD = -std=gnu11
WARNINGS = -Wall -Werror -Wextra
INCLUDES = -Ilibc/include
DEBUG_FLAGS = -O0 -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# DEBUG/RELEASE
ifeq ($(mode), release)
	CFLAGS += -ffreestanding -fbuiltin $(WARNINGS) $(RELEASE_FLAGS) $(INCLUDES)
else
	mode = debug
	CFLAGS += -ffreestanding -fbuiltin $(WARNINGS) $(DEBUG_FLAGS) $(INCLUDES)
endif
	
PREFIX = /usr
EXEC_PREFIX = ${PREFIX}
BOOTDIR	= /boot
LIBDIR = ${EXEC_PREFIX}/lib
INCLUDEDIR=${PREFIX}/include

DESTDIR = $(PWD)/sysroot

$(CC) = "${CC} --sysroot=${PWD}/sysroot -isystem=${INCLUDEDIR}"

all: information all-libc all-kernel

information:
# TODO: Make this accept a list of acceptable architectures
ifneq ($(arch),i386)
	@echo $(error Please specify architecture. 'make arch=PREFERRED_ARCHITECTURE')
endif
	@echo "Building for "$(arch)" architecture"
	@echo ".........................."

run: all
	qemu-system-$(HOSTARCH) -monitor stdio -kernel build/kernel.elf

# TODO: If no target architecture specified do dialog to ask whether to clean all or not
clean: clean-libc clean-kernel
	find sysroot/usr/include -name '*.h' -type f -delete

# MAKEFILE DEBUGGING STUFF

# Print makefile variables
print-%  : ; @echo $* = $($*)
