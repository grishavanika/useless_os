# GNU make: https://www.gnu.org/software/make/manual/make.html
CXX_=i686-elf-g++
AS_=i686-elf-as
OBJS:=build/boot.o build/kernel.o

# Objects to handle proper global c-tors/d-tors.
CRTI_OBJ=build/crti.o
CRTBEGIN_OBJ:=$(shell $(CXX_) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX_) -print-file-name=crtend.o)
CRTN_OBJ=build/crtn.o

# Objects must be linked in this exact order.
OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: directories \
	build/useless_os.bin \
	build/useless_os.iso

.PHONY: directories
directories:
	mkdir -p build

build/useless_os.bin: $(OBJ_LINK_LIST)
	$(CXX_) -T linker.ld -o build/useless_os.bin \
		-ffreestanding -O2 -nostdlib -lgcc \
		$(OBJ_LINK_LIST)
	./check_multiboot_format.sh

build/useless_os.iso: build/useless_os.bin grub.cfg
	mkdir -p build/iso_pack/boot/grub
	cp build/useless_os.bin build/iso_pack/boot/useless_os.bin
	cp grub.cfg build/iso_pack/boot/grub/grub.cfg
	grub-mkrescue -o build/useless_os.iso build/iso_pack

build/crti.o: src/x86_crti.s
	$(AS_) src/x86_crti.s -o build/crti.o

build/boot.o: src/boot.s
	$(AS_) src/boot.s -o build/boot.o

build/kernel.o: src/kernel.cpp
	$(CXX_) -c src/kernel.cpp -o build/kernel.o \
		-std=c++17 -ffreestanding -O2 \
		-Wall -Wextra -fno-exceptions -fno-rtti

build/crtn.o: src/x86_crtn.s
	$(AS_) src/x86_crtn.s -o build/crtn.o

.PHONY: clean
clean:
	rm -rf build
