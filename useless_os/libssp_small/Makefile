SYSROOT=../sysroot
PREFIX=/usr
DESTDIR=$(SYSROOT)
INCLUDEDIR=$(PREFIX)/include
LIBDIR=$(PREFIX)/lib

CXX_=i686-elf-g++ --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)
AR_=i686-elf-ar

OBJS=\
build/stack_chk_fail.cpp.o \

.PHONY: all
all: build/libssp_small.a

# $@ is the name of the target (libc_small.a in this case).
# AR rcs - basically, create static (archive) library.
build/libssp_small.a: $(OBJS)
	mkdir -p build
	$(AR_) rcs build/libssp_small.a $(OBJS)

build/stack_chk_fail.cpp.o: stack_chk_fail.cpp
	mkdir -p build
	$(CXX_) -c stack_chk_fail.cpp -o build/stack_chk_fail.cpp.o \
	-std=c++20 -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
	-ffreestanding

.PHONY: clean
clean:
	rm -rf build

.PHONY: install
install: install-libs

install-libs: build/libssp_small.a
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp build/libssp_small.a $(DESTDIR)$(LIBDIR)
