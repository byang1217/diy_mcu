#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

STM32_LIB_PATH := $(CURDIR)/STM32F10x_StdPeriph_Lib_V3.5.0

TARGET := stm32f10x
CROSS_COMPILE = arm-none-eabi-
CFLAGS += -I$(CURDIR) -I$(STM32_LIB_PATH) -mcpu=cortex-m3 -mthumb -msoft-float
LDFLAGS += -mthumb -mthumb-interwork
LDFLAGS += -Wl,-Map=$(OUT)/$(TARGET).map -T $(OUT)/ld.script 
LD_SCRIPT := bsp/xip.lds.S

SOURCES += $(CURDIR)/bsp.c

# check by st-info --probe
# stm32f10x_ld stm32f10x_ld_vl stm32f10x_md stm32f10x_md_vl stm32f10x_hd stm32f10x_hd_vl stm32f10x_xl stm32f10x_cl
ifeq ($(SOC),stm32f10x_md)
CFLAGS += -DSTM32F10X_MD
SOURCES += $(STM32_LIB_PATH)/startup/startup_stm32f10x_md.s
endif

SOURCES += $(STM32_LIB_PATH)/system_stm32f10x.c
SOURCES += $(STM32_LIB_PATH)/core_cm3.c
SOURCES += $(STM32_LIB_PATH)/misc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_adc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_bkp.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_can.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_cec.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_crc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_dac.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_dbgmcu.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_dma.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_exti.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_flash.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_fsmc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_gpio.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_i2c.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_iwdg.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_pwr.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_rcc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_rtc.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_sdio.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_spi.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_tim.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_usart.c
SOURCES += $(STM32_LIB_PATH)/stm32f10x_wwdg.c

all: $(OUT)/$(TARGET).bin
install: install-stlink
debug: debug-stlink

$(OUT)/$(TARGET).bin: $(OUT)/$(TARGET).elf
	$(Q)echo create $@
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)$(SIZE) $<

$(OUT)/$(TARGET).elf: $(OBJS) $(OUT)/ld.script
	$(Q)echo create $@
	$(Q)$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(Q)$(OBJDUMP) -SD $@ > $(OUT)/$(TARGET).lst

install-isp: $(OUT)/$(TARGET).bin
	$(Q)echo flash $<
	$(Q)stm32flash /dev/ttyUSB0 -b 9600
	$(Q)stm32flash -w $< -v -g 0x0 /dev/ttyUSB0 -b 9600

install-stlink: $(OUT)/$(TARGET).bin
	$(Q)echo flash $<
	$(Q)st-info --probe
	$(Q)st-flash write $< 0x8000000

debug-stlink:
	$(Q)killall -9 st-util 2>/dev/null || true
	$(Q)st-info --probe
	$(Q)st-util > /dev/null &
	$(Q)$(GDB) $(OUT)/$(TARGET).elf -ex "target remote :4242"
	$(Q)killall -9 st-util 2>/dev/null || true

