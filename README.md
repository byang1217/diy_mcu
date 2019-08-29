# stm32

## toolchain
https://github.com/stm32duino/arm-none-eabi-gcc/releases/download/6-2017-q2-update/gcc-arm-none-eabi-6-2017-q2-update-linux64.tar.bz2
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

## stlink utils
git clone https://github.com/texane/stlink.git

apt-get install libusb-1.0.0-dev pkg-config build-essential libgtk-3-dev

cmake .

sudo make install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

## stm32 hw lib
https://www.stmicroelectronics.com.cn/en/embedded-software/stm32-standard-peripheral-libraries.html


# STC15

## toolchain
sdcc

## flash tool
https://github.com/grigorig/stcgal
(apt-get install python3-tqdm python3-serial)
