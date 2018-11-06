# my_stm32

## toolchain
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

## stlink utils
git clone https://github.com/texane/stlink.git

apt-get install libusb-1.0.0-dev pkg-config build-essential libgtk-3-dev

cmake .

sudo make install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

## stm32 hw lib
https://www.stmicroelectronics.com.cn/en/embedded-software/stm32-standard-peripheral-libraries.html
