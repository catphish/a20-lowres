set -e
arm-none-eabi-gcc -T linker.ld -mcpu=cortex-a7 -fpic -ffreestanding -O2 -nostdlib  -o myos.elf boot.s display.c uart.c kernel.c
arm-none-eabi-objcopy -O binary myos.elf myos.bin
#sudo cp ./myos.elf /var/lib/tftpboot/C0A80002.img
echo OK
