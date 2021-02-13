Everything is from <https://wiki.osdev.org/>.  

### Building a GCC Cross-Compiler

  * GCC Cross-Compiler for i686-elf. We need it because:

    * i686-elf is a generic target that provides you with a
      toolchain targeting the [System V ABI](https://wiki.osdev.org/System_V_ABI);
    * ... that allows you to easily set up a bootable kernel
      using GRUB and [Multiboot](https://en.wikipedia.org/wiki/Multiboot_specification);
    * x86_64-elf cross-compiler is not okay because GRUB
      (implementation of Multiboot Specification) is only able to load 32-bit multiboot kernels

  * default GCC compiler most likely produces binaries for x86_64;  
    as stated above GRUB will not be happy with this. To check your compiler:
    ```
    gcc -dumpmachine
    # output (for Cygwin environment)
    x86_64-pc-cygwin
    ```

See [Why do I need a Cross Compiler?](https://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F).  
See also [Target Triplet](https://wiki.osdev.org/Target_Triplet) to know
more about what "i686-elf-gcc" means.  

TL;DR steps:

  1. build binutils: binutils (assembler, disassembler and so on) that understand
     how to build/target i686-elf. It also needed to build GCC for i686-elf.
  2. build GCC for i686-elf.

### Building a GCC Cross-Compiler on Windows with WSL

```
# WSL2 install: https://docs.microsoft.com/en-us/windows/wsl/install-win10
# dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
# dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
# restart
# wsl --set-default-version 2
# install Ubuntu from https://aka.ms/wslstore
# enable clipboard cop-paste: https://devblogs.microsoft.com/commandline/copy-and-paste-arrives-for-linuxwsl-consoles
# 
bash.exe
cd $HOME
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.36.tar.gz
curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz
mkdir src
tar -xzf gcc-10.2.0.tar.gz -C src
tar -xzf binutils-2.36.tar.gz -C src
# binutils
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/src
mkdir build-binutils
cd build-binutils
# GCC & make to build binutils
sudo apt-get install gcc
sudo apt-get install gcc++
sudo apt install make
../binutils-2.36/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror
# or make -j`nproc`
make -j8
make install
# gcc
cd $HOME/src/gcc-10.2.0
./contrib/download_prerequisites
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/src
mkdir build-gcc
cd build-gcc
# check binutils install
which -- $TARGET-as || echo $TARGET-as is not in the PATH
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" \
    --disable-nls --enable-languages=c,c++ --without-headers
make -j8 all-gcc
make -j8 all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
$HOME/opt/cross/bin/$TARGET-gcc --version
export PATH="$HOME/opt/cross/bin:$PATH"
```

Add `i686-elf-gcc` to `.bashrc` (`nano ~/.bashrc`):

```
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export PATH="$HOME/opt/cross/bin:$PATH"
$TARGET-gcc --version
$TARGET-gcc -dumpmachine
```

### Booting 

  * (Windows, GUI) Install [QEMU](https://www.qemu.org/download/#windows);
    add installation folder to PATH.
  * WSL: make sure `which qemu-system-i386` works; install if missing.
  * Build sources (see [Bare Bones](https://wiki.osdev.org/Bare_Bones)):
    ```
    # install once
    # sudo apt install grub-common
    # sudo apt-get install xorriso
    # sudo apt install qemu-system-i386
    # 
    # build useless_os.bin
    # cd /mnt/c/dev/useless_os/bare_bones
    i686-elf-as boot.s -o boot.o
    i686-elf-g++ -c kernel.cpp -o kernel.o -std=c++17 -ffreestanding -O2 \
        -Wall -Wextra -fno-exceptions -fno-rtti
    i686-elf-g++ -T linker.ld -o useless_os.bin -ffreestanding -O2 \
        -nostdlib boot.o kernel.o
    # 
    # build useless.iso
    mkdir -p isodir/boot/grub
    cp useless_os.bin isodir/boot/useless_os.bin
    cp grub.cfg isodir/boot/grub/grub.cfg
    grub-mkrescue -o useless_os.iso isodir
    ```

  * Run on Windows:
    ```
    qemu-system-i386.exe -kernel useless_os.bin
    # run with
    #   qemu-system-i386.exe -kernel useless_os.bin -L C:\Programs\qemu
    # (where 'C:\Programs\qemu' is path to QEMU installation)
    # in case of "qemu: could not load PC BIOS 'bios-256k.bin'" error.
    ```
  * Run inside WSL (warning: need to terminate process):
    ```
    qemu-system-i386 -curses -cdrom useless_os.iso
    qemu-system-i386 -curses -kernel useless_os.bin
    ```

