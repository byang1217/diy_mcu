#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

TARGET := stm32f10x
CROSS_COMPILE = arm-none-eabi-
CFLAGS += -I$(CURDIR) -mcpu=cortex-m3 -mthumb -msoft-float
LDFLAGS += -mthumb -mthumb-interwork
LDFLAGS += -Wl,-Map=$(OUT)/$(TARGET).map -T $(OUT)/ld.script 
LD_SCRIPT := bsp/xip.lds.S

SOURCES += $(CURDIR)/bsp.c

all: $(OUT)/$(TARGET).bin
install: install-stlink
debug: debug-stlink

$(OUT)/$(TARGET).bin: $(OUT)/$(TARGET).elf
	$(Q)echo create $@
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)$(SIZE) $<

$(OUT)/$(TARGET).elf: $(OUT)/version.h $(OBJS) $(OUT)/ld.script
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

