# make install-headers
# make
# OR
# make install
SYSROOT=../sysroot
PREFIX=/usr
DESTDIR=$(SYSROOT)
INCLUDEDIR=$(PREFIX)/include
LIBDIR=$(PREFIX)/lib

CXX_=i686-elf-g++ --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)
AR_=i686-elf-ar

OBJS=\
build/stdio.cpp.o \
build/stdlib.cpp.o \
build/string.cpp.o \

.PHONY: all
all: build/libc_small.a

# $@ is the name of the target (libc_small.a in this case).
# AR rcs - basically, create static (archive) library.
build/libc_small.a: $(OBJS)
	mkdir -p build
	$(AR_) rcs build/libc_small.a $(OBJS)

build/stdio.cpp.o: stdio.cpp
	mkdir -p build
	$(CXX_) -c stdio.cpp -o build/stdio.cpp.o \
	-std=c++20 -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
	-ffreestanding \
	-fstack-protector-all
build/stdlib.cpp.o: stdlib.cpp
	mkdir -p build
	$(CXX_) -c stdlib.cpp -o build/stdlib.cpp.o \
	-std=c++20 -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
	-ffreestanding \
	-fstack-protector-all
build/string.cpp.o: string.cpp
	mkdir -p build
	$(CXX_) -c string.cpp -o build/string.cpp.o \
	-std=c++20 -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
	-ffreestanding \
	-fstack-protector-all

.PHONY: clean
clean:
	rm -rf build

.PHONY: install
install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: build/libc_small.a
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp build/libc_small.a $(DESTDIR)$(LIBDIR)
