#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

STM32_LIB_PATH := $(CURDIR)/STM32_USB-FS-Device_Lib_V4.1.0

CFLAGS += -I$(CURDIR) -I$(STM32_LIB_PATH) -I$(STM32_LIB_PATH)/Libraries/CMSIS/Include

# check by st-info --probe
# stm32f10x_ld stm32f10x_ld_vl stm32f10x_md stm32f10x_md_vl stm32f10x_hd stm32f10x_hd_vl stm32f10x_xl stm32f10x_cl
ifeq ($(SOC),stm32f10x_md)
CFLAGS += -DSTM32F10X_MD -I$(STM32_LIB_PATH)/Libraries/CMSIS/Device/ST/STM32F10x/Include -I$(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/inc
SOURCES += $(STM32_LIB_PATH)/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/gcc/startup_stm32f10x_md.s
SOURCES += $(STM32_LIB_PATH)/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/system_stm32f10x.c
STM32_SRC_PREFIX := stm32f10x_
STM32_DRV_PATH := $(STM32_LIB_PATH)/Libraries/STM32F10x_StdPeriph_Driver
CFLAGS += -I$(STM32_DRV_PATH)/inc
endif

SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c
SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c
SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c
SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c
SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c
SOURCES += $(STM32_LIB_PATH)/Libraries/STM32_USB-FS-Device_Driver/src/usb_sil.c

SOURCES += $(STM32_DRV_PATH)/src/misc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)adc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)bkp.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)can.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)cec.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)crc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)dac.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)dbgmcu.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)dma.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)exti.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)flash.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)fsmc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)gpio.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)i2c.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)iwdg.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)pwr.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)rcc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)rtc.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)sdio.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)spi.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)tim.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)usart.c
SOURCES += $(STM32_DRV_PATH)/src/$(STM32_SRC_PREFIX)wwdg.c
