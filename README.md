# Bare Shell
Simplest shell for bare-metal

----
## Environment Setup 
### Install Toolchain
Download toolchain from [Linaro](https://www.linaro.org/downloads/). Please select arm-eabi, the toolchain for bare-metal

    mkdir ~\toolchain
    tar xf gcc-linaro-7.2.1-2017.11-x86_64_arm-eabi.tar.xz -C ~\toolchain
    export PATH=~/toolchain/gcc-linaro-7.2.1-2017.11-x86_64_arm-eabi/bin:$PATH

### Install Qemu
    sudo apt-get install qemu-system-arm
    

----
## Compile
    make

----
## Run
    ./run.sh

    Welcome to Bare Shell, a tiny shell for bare metal

    Bell>

    Bell>help
    hello      Just print hello
    echo      Echo parameters
    help      Print command help info
    
    # to quit
    # Ctrl-A then x
