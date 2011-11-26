# Standard things
sp				:= $(sp).x
dirstack_$(sp)	:= $(d)
d				:= $(dir)

# Subdirectories, in no particular order
dir				:= $(d)/core
include			$(dir)/rules.mk
dir				:= $(d)/vendor
include			$(dir)/rules.mk

# standard things
d				:= $(dirstack_$(sp))
sp				:= $(basename $(sp))
