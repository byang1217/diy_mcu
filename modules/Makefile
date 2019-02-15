#
# Copyright (C) <2018>  Bin Yang <byang1217@gmail.com>
#

MODDIR := $(CURDIR)

define mod_template
CURDIR := $$(MODDIR)/$(1)
gdeps += $$(CURDIR)/Makefile
CFLAGS += -DMOD_$(1)
-include $$(CURDIR)/Makefile
endef

$(foreach mod,$(MODULES),$(eval $(call mod_template,$(mod))))
