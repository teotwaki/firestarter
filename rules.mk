# Standard things
.SUFFIXES:
.SUFFIXES:	.cpp .o

all:		targets

# Subdirectories, in no specific order

dir			:= src
include		$(dir)/rules.mk

# General directory-independent rules

%.o:		%.cpp
			$(COMP)

%.so:		%.o
			$(SHAREDLINK)

%:			%.o
			$(LINK)

%.d:		%.o
			$(COMP)

# The variables TGT_* and CLEAN may be added by the sub-directory
# Makefiles

debug:	CF_ALL := $(CF_ALL) -g -Wextra
debug:	LF_ALL := $(LF_ALL) -g -Wextra
debug:	targets

.PHONY:		targets
targets:	$(TGT_BIN) $(TGT_AR)

.PHONY:		clean
clean:
	rm -f $(CLEAN)

# Prevent make from removing any build targets, including intermediate ones
.SECONDARY:		$(CLEAN)

