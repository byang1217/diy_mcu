#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

DEBUG		:=n
V		:=0
ifeq ($(V), 1)
export JOBS	:= 1
export Q	:=
export MAKEFLAGS :=
else
export JOBS	:= $(shell grep --count processor /proc/cpuinfo)
export Q	:=@
export MAKEFLAGS := -s --no-print-directory
endif

APP ?=

APPS_LIST = $(shell ls apps/)
PHONY += $(APPS_LIST)

ifneq ($(MAKECMDGOALS),help)
ifeq ($(filter $(APP), $(APPS_LIST)),)
$(info Need to export APP=<app>)
$(info "Available APP list: $(APPS_LIST)")
endif
endif

TOP := $(shell pwd)
OUT := out/$(APP)

CFLAGS := -g -Werror -ffunction-sections -fdata-sections
ifeq ($(DEBUG),y)
CFLAGS += -O0
else
CFLAGS += -Os
endif
LDFLAGS := -nostdlib -static -Wl,--gc-sections

gdeps := Makefile
SOURCES :=
OBJS =
OBJS += $(patsubst %.c,$(OUT)/%.o,$(filter %.c, $(SOURCES)))
OBJS += $(patsubst %.s,$(OUT)/%.o,$(filter %.s, $(SOURCES)))
OBJS += $(patsubst %.S,$(OUT)/%.o,$(filter %.S, $(SOURCES)))

# keep app Makefile at first
CURDIR := apps/$(APP)
gdeps += $(CURDIR)/Makefile
CFLAGS += -DAPP=$(APP)
-include $(CURDIR)/Makefile

CURDIR := common
gdeps += $(CURDIR)/Makefile
-include $(CURDIR)/Makefile

# keep bsp Makefile at lastest
CURDIR := bsp/$(BSP)
gdeps += $(CURDIR)/Makefile
CFLAGS += -DBSP=$(BSP)
-include $(CURDIR)/Makefile

PHONY += all install debug
all:
install:
debug:

tags:
	find apps common bsp/$(BSP) -name "*.[cSsh]" -o -name "*.cpp" -o -name "*.inc" > cscope.files
	cscope -kbq

-include $(patsubst %.o,%.dep,$(filter %.o, $(OBJS)))

PHONY += help
help:
	@echo  "make <app>			- To build specific app"
	@echo  "make clean			- Remove all generated files"
	@echo  "make <app> V=0|1		- 0: quiet build, 1: verbose build"
	@echo
	@echo "Apps list:"
	for app in $(APPS_LIST) ;\
	do	\
		echo "	"$$app;\
	done
	@echo  ''
	@echo  'Copyright (C) Bin Yang <byang1217@gmail.com>'
	@echo  ''

export ASM	= $(CROSS_COMPILE)gcc
export LD	= $(CROSS_COMPILE)ld
export CC	= $(CROSS_COMPILE)gcc
export CPP	= $(CROSS_COMPILE)gcc
export AR	= $(CROSS_COMPILE)ar
export OBJDUMP	= $(CROSS_COMPILE)objdump
export OBJCOPY	= $(CROSS_COMPILE)objcopy
export SIZE	= $(CROSS_COMPILE)size
export GDB	= $(CROSS_COMPILE)gdb

.SUFFIXES :
.SUFFIXES : .o .s .S .c
$(OUT)/%.o: %.c $(gdeps)
	$(Q)echo "CC $<"
	mkdir -p `dirname $@`
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@:+' $(patsubst %.o,%.dep,$@)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.s $(gdeps)
	$(Q)echo "CC $<"
	mkdir -p `dirname $@`
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@+' $(patsubst %.o,%.dep,$@)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.S $(gdeps)
	$(Q)echo "CC $<"
	mkdir -p `dirname $@`
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@+' $(patsubst %.o,%.dep,$@)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/ld.script: $(LD_SCRIPT) $(gdeps)
	$(Q)echo "Gen ld.script"
	$(Q)$(CC) -E $(CFLAGS) -P -o $@ $<

PHONY += clean
clean:
	rm -rf $(OUT)
