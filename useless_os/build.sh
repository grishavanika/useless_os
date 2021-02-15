# Headers to $SYSROOT.
cd libc_small
make install-headers
cd ..
cd kernel
make install-headers
cd ..

# Build all.
cd libssp_small
make -B install
cd ..
cd libc_small
make -B install
cd ..
cd kernel
make -B install
cd ..
