# Build flags for all targets
ifeq ($(BUILDTYPE),debug)
	CF_ALL		= -pipe -Wall -g -Wextra
else
	CF_ALL		= -pipe -Wall
endif
LF_ALL		= -pipe
LL_ALL		= 

# Build tools
CC			:= ./build/dep-g++
COMP		= $(CC) $(CF_ALL) $(CF_LOC) $(CF_TGT) -o $@ -c $< -fpic
LINK		= $(CC) -Wl,--export-dynamic $(LF_ALL) -o $@ $^ $(LL_TGT) $(LL_ALL) $(LF_TGT) 
SHAREDLINK	= $(CC) -Wl,--export-dynamic -shared $(LF_ALL) -o $@ $^ $(LL_TGT) $(LL_ALL) $(LF_TGT)
COMPLINK	= $(CC) $(CF_ALL) $(CF_LOC) $(CF_TGT) $(LF_ALL) $(LF_TGT) -o $@ $< $(LL_TGT) $(LL_ALL)
COPYTO		= cp $@
COPY		= $(COPYTO) bin/

# Standard parts
include rules.mk
