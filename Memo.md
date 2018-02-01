# Porting of Vexpress and Versitlepb on Qemu


## Serial Port Init of Qemu
Only one serial port is actually created

    add_device_config(DEV_SERIAL, "mon:stdio");
    foreach_device_config(DEV_SERIAL, serial_parse)

    serial_parse
      static int index = 0;
      serial_hds[index] = qemu_chr_new(label, devname);


## PL011 UART device creation for Versitile
The device is memory mapped to 0x101f1000

    pl011_create(0x101f1000, pic[12], serial_hds[0]);


## PL011 UART device creation for Vexpress 
Similar to above, vexpress's PL011 is mapped to 0x10009000# Porting of Vexpress and Versitlepb on Qemu


## Serial Port Init of Qemu
Only one serial port is actually created

    add_device_config(DEV_SERIAL, "mon:stdio");
    foreach_device_config(DEV_SERIAL, serial_parse)

    serial_parse
      static int index = 0;
      serial_hds[index] = qemu_chr_new(label, devname);


## PL011 UART device creation for Versitile
The device is memory mapped to 0x101f1000

    pl011_create(0x101f1000, pic[12], serial_hds[0]);

## PL011 UART device creation for Vexpress 
Similar to above, vexpress's PL011 is mapped to 0x10009000. 


​​## DRAM Address
Vexpress example.   
Board set the loader address at, like in the vexpress.c

    .loader_start = 0x60000000,
    memory_region_add_subregion(sysmem, 0x60000000, ram);


# Qemu kernel loading
The kernel is loaded to load_start + KERNEL_LOAD_ADDR, which is 0x100000​​

    entry = info->loader_start + KERNEL_LOAD_ADDR;
    kernel_size = ​​load_image_targphys(info->kernel_filename, entry, info->ram_size - KERNEL_LOAD_ADDR);
​    rom_add_file_fixed_as() // which put the address to rom​
. 

## DRAM Address setting
Vexpress example  
Board set the loader address at, like in the vexpress.c

    .loader_start = 0x60000000,
    memory_region_add_subregion(sysmem, 0x60000000, ram);


# Qemu kernel loading
The kernel is loaded to load_start + KERNEL_LOAD_ADDR, which is 0x100000​​

    entry = info->loader_start + KERNEL_LOAD_ADDR;
    kernel_size = ​​load_image_targphys(info->kernel_filename, entry, info->ram_size - KERNEL_LOAD_ADDR);
​    rom_add_file_fixed_as() // which put the address to rom​
