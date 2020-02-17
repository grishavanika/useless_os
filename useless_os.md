Everything is from <https://wiki.osdev.org/>  

### Building a GCC Cross-Compiler

  * GCC Cross-Compiler for i686-elf. We need it because:
    * i686-elf is a generic target that provides you with a
      toolchain targeting the System V ABI;
    * ... that allows you to easily set up a bootable kernel
      using GRUB and Multiboot;
    * x86_64-elf cross-compiler is not okay because
      GRUB is only able to load 32-bit multiboot kernels

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
bash.exe
cd $HOME
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz
curl -O https://ftp.gnu.org/gnu/gcc/gcc-9.2.0/gcc-9.2.0.tar.gz
mkdir src
tar -xzf gcc-9.2.0.tar.gz -C src
tar -xzf binutils-2.34.tar.gz -C src
# binutils
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/src
mkdir build-binutils
cd build-binutils
../binutils-2.34/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror
make -j8
make install
# gcc
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
cd $HOME/src
mkdir build-gcc
cd build-gcc
which -- $TARGET-as || echo $TARGET-as is not in the PATH
../gcc-9.2.0/configure --target=$TARGET --prefix="$PREFIX" \
 --disable-nls --enable-languages=c,c++ --without-headers
make -j8 all-gcc
make -j8 all-target-libgcc
make install-gcc
make install-target-libgcc
$HOME/opt/cross/bin/$TARGET-gcc --version
export PATH="$HOME/opt/cross/bin:$PATH"
```

Add `i686-elf-gcc` to `.bashrc` (nano `~/.bashrc`):

```
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export PATH="$HOME/opt/cross/bin:$PATH"
$TARGET-gcc --version
$TARGET-gcc -dumpmachine
```

#### Building a GCC Cross-Compiler on Windows with Cygwin environment

[All steps](https://wiki.osdev.org/GCC_Cross-Compiler):

  1. Download & install [Cygwin](https://cygwin.com/install.html)
  2. Make sure all packages are downloaded and installed after initial installation.
     Just re-run Cygwin installer and select next packages:
      * gcc-g++ (instead of mingw64-x86_64-gcc-g++).  
        This will install binary with exactly "gcc" name;
        mingw64-x86_64-gcc-g++ requires additional steps to be recognized by
        ./configure scripts (? like making an alias ?)
      * make; bison; flex; libgmp-devel; libmpc-devel
        libmpfr-devel; texinfo; libisl-devel

     Note: libcloog was missing from packages list. However everything was
     good without it.  
     [TODO]: why exactly we need all of those packages ? GCC build requirements ?
  3. Download and unpack binutils and GCC sources to ~/src.  
     Binutils can be found [there](https://www.gnu.org/software/binutils/);
     [direct link](https://ftp.gnu.org/gnu/binutils/).  
     GCC can be found [there](https://www.gnu.org/software/gcc/);
     [direct link](https://ftp.gnu.org/gnu/gcc).  
     In order to unpack tar.gz use:
     ```
     mkdir src
     tar -xzf gcc-9.2.0.tar.gz -C src
     tar -xzf binutils-2.33.1.tar.gz -C src
     ```
  4. Build binutils. In case it's binutils-2.33.1:
     ```
     export PREFIX="$HOME/opt/cross"
     export TARGET=i686-elf
     export PATH="$PREFIX/bin:$PATH"
     cd $HOME/src
     mkdir build-binutils
     cd build-binutils
     ../binutils-2.33.1/configure --target=$TARGET --prefix="$PREFIX" \
         --with-sysroot --disable-nls --disable-werror
     # run make using 8 cores
     make -j8
     # don't forget to install !
     make install
     ```
  5. Build GCC. In case it's :
     ```
     export PREFIX="$HOME/opt/cross"
     export TARGET=i686-elf
     export PATH="$PREFIX/bin:$PATH"
     cd $HOME/src
     mkdir build-gcc
     cd build-gcc
     which -- $TARGET-as || echo $TARGET-as is not in the PATH
     ../gcc-9.2.0/configure --target=$TARGET --prefix="$PREFIX" \
         --disable-nls --enable-languages=c,c++ --without-headers
     make -j8 all-gcc
     make -j8 all-target-libgcc
     make install-gcc
     make install-target-libgcc
     $HOME/opt/cross/bin/$TARGET-gcc --version
     export PATH="$HOME/opt/cross/bin:$PATH"
     ```
     See [GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler#Binutils)
     for `--disable-nls` and other parameters explanations.
  6. Crete next bash script in $HOME directory (for example, setup_dev_compiler.sh):

     ```
     #!/bin/bash
     export PREFIX="$HOME/opt/cross"
     export TARGET=i686-elf
     export PATH="$PREFIX/bin:$PATH"
     export PATH="$HOME/opt/cross/bin:$PATH"
     $TARGET-gcc --version
     $TARGET-gcc -dumpmachine
     ```
      * make sure it's saved with unix lines endings (Sublime/View/Line Endings/Unix)
      * probably you need to `chmod +x setup_dev_compiler.sh` to allow execution
      * after running Cygwin.bat - execute `./setup_dev_compiler.sh`.  
        No errors should exist. Output should contain **i686-elf**.  

     Alternatively, you can add those lines to .bash_profile or .profile 
     to execute them manually when Cygwin.bat pressed.

### Booting 

  * (Windows, GUI) Install [QEMU](https://www.qemu.org/download/#windows).
  * WSL: make sure `which qemu-system-i386` works; install if missing.
  * Build sources (see [Bare Bones](https://wiki.osdev.org/Bare_Bones)):
    ```
    # build useless_os.bin
    i686-elf-as boot.s -o boot.o
    i686-elf-g++ -c kernel.cpp -o kernel.o -std=c++17 -ffreestanding -O2 \
        -Wall -Wextra -fno-exceptions -fno-rtti
    i686-elf-g++ -T linker.ld -o useless_os.bin -ffreestanding -O2 \
        -nostdlib boot.o kernel.o
    
    # build useless.iso
    mkdir -p isodir/boot/grub
    cp useless_os.bin isodir/boot/useless_os.bin
    cp grub.cfg isodir/boot/grub/grub.cfg
    grub-mkrescue -o useless_os.iso isodir
    ```

  * Run on Windows:
    ```
    qemu-system-i386.exe -kernel useless_os.bin
    ```
  * Run inside WSL (warning: need to terminate process):
    ```
    qemu-system-i386 -curses -cdrom useless_os.iso
    qemu-system-i386 -curses -kernel useless_os.bin
    ```

