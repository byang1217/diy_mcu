#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

TARGET := linux_app
CROSS_COMPILE = ""
CFLAGS += -O0
CFLAGS += -I$(CURDIR)
CFLAGS += -DLINUX_APP
LDFLAGS := $(filter-out -nostdlib -static, $(LDFLAGS))

SOURCES += $(CURDIR)/bsp.c

all: $(OUT)/$(TARGET)
debug: $(OUT)/$(TARGET)

$(OUT)/$(TARGET): $(OUT)/version.h $(OBJS)
	$(Q)echo create $@
	$(Q)$(CC) -o $@ $(OBJS) $(LDFLAGS)

