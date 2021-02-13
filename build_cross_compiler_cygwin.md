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
     tar -xzf gcc-10.2.0.tar.gz -C src
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
     ../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" \
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

