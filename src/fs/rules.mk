# Standard things

sp				:= $(sp).x
dirstack_$(sp)	:= $(d)
d				:= $(dir)

# Local rules and targets

CF_$(d)			:= $(shell pkg-config libconfig++ --cflags) $(shell pkg-config liblog4cxx --cflags) -Isrc/common
LF_$(d)			:= $(shell pkg-config libconfig++ --libs) $(shell pkg-config liblog4cxx --libs)

TGTS_$(d)		:= $(d)/firestarter
OBJS_$(d)		:= $(d)/main.o src/common/helper.o src/common/log.o
				   
DEPS_$(d)		:= $(OBJS_$(d):%=%.d)

TGT_BIN			:= $(TGT_BIN) $(TGTS_$(d))
CLEAN			:= $(CLEAN) $(TGTS_$(d)) $(OBJS_$(d)) $(DEPS_$(d))

$(TGTS_$(d)):	CF_TGT := $(CF_$(d))
$(TGTS_$(d)):	LF_TGT := $(LF_$(d))
$(TGTS_$(d)):	$(OBJS_$(d))
				$(LINK)
				$(COPY)

# Standard things

-include		$(DEPS_$(d))
CF_TGT 			:=
LF_TGT			:=
d				:= $(dirstack_$(sp))
sp				:= $(basename $(sp))
