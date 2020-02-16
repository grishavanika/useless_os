if grub-file --is-x86-multiboot useless_os.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi
