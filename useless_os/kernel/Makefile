# GNU make: https://www.gnu.org/software/make/manual/make.html
SYSROOT=../sysroot
PREFIX=/usr
DESTDIR=$(SYSROOT)
INCLUDEDIR=$(PREFIX)/include


CXX_=i686-elf-g++ --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)
AS_=i686-elf-as
OBJS:=\
	build/arch/i386/vga_text.o \
	build/arch/i386/boot.o \
	build/kernel.o

# Objects to handle proper global c-tors/d-tors.
CRTI_OBJ=build/arch/i386/crti.o
CRTBEGIN_OBJ:=$(shell $(CXX_) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX_) -print-file-name=crtend.o)
CRTN_OBJ=build/arch/i386/crtn.o

# Objects must be linked in this exact order.
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: \
	build/useless_os.bin \
	build/useless_os.iso

build/useless_os.bin: $(OBJ_LINK_LIST)
	mkdir -p build/arch/i386
	$(CXX_) -T arch/i386/linker.ld -o build/arch/i386/useless_os.bin \
		-ffreestanding -O2 -nostdlib -lgcc \
		$(OBJ_LINK_LIST) \
		-lc_small
	grub-file --is-x86-multiboot build/arch/i386/useless_os.bin

build/useless_os.iso: build/useless_os.bin arch/i386/grub.cfg
	mkdir -p build/iso_pack/boot/grub
	cp build/arch/i386/useless_os.bin build/iso_pack/boot/useless_os.bin
	cp arch/i386/grub.cfg build/iso_pack/boot/grub/grub.cfg
	grub-mkrescue -o build/useless_os.iso build/iso_pack

build/arch/i386/crti.o: arch/i386/x86_crti.s
	mkdir -p build/arch/i386
	$(AS_) arch/i386/x86_crti.s -o build/arch/i386/crti.o

build/arch/i386/boot.o: arch/i386/boot.s
	mkdir -p build/arch/i386
	$(AS_) arch/i386/boot.s -o build/arch/i386/boot.o

build/kernel.o: kernel.cpp
	mkdir -p build/arch/i386
	$(CXX_) -c kernel.cpp -o build/kernel.o \
		-std=c++20 -ffreestanding -O2 \
		-Iinclude \
		-Wall -Wextra -fno-exceptions -fno-rtti

build/arch/i386/vga_text.o: arch/i386/vga_text.cpp
	mkdir -p build/arch/i386
	$(CXX_) -c arch/i386/vga_text.cpp -o build/arch/i386/vga_text.o \
		-std=c++20 -ffreestanding -O2 \
		-Iinclude \
		-Wall -Wextra -fno-exceptions -fno-rtti

build/arch/i386/crtn.o: arch/i386/x86_crtn.s
	mkdir -p build/arch/i386
	$(AS_) arch/i386/x86_crtn.s -o build/arch/i386/crtn.o

install: install-headers build/useless_os.iso

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

.PHONY: clean
clean:
	rm -rf build
