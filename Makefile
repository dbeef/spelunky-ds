#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source source/tiles source/entities/_interfaces source/console source/input  \
                source/collisions source/entities source/entities/treasures source/entities/creatures \
                source/entities/items source/entities/decorations source/time \
                source/hud source/rooms source/memory source/camera source/entities/animations \
                source/entities/main_dude source/sound source/entities/traps preprocessor/Debug.h
DATA		:=	data
INCLUDES	:=	include
FONTS   	:=	gfx/fonts
SPRITES	    :=	gfx/sprites/64x64 gfx/sprites/8x8 gfx/sprites/32x32 gfx/sprites/16x16
TILEMAPS    :=  gfx/tilemaps
SOUNDS      :=  sounds
SOUNDBANK_NAME := soundbank
GAME_ICON := ../gfx/icon/icon.bmp
GAME_TITLE := SpelunkyDS
GAME_SUBTITLE1 := github.com/dbeef/spelunky-ds
GAME_SUBTITLE2 :=
NITRODATA := nitrofiles

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O2\
 			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer\
			-ffast-math \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=c++14

ifndef DEBUG
DEBUG = 0
endif

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -DDEBUG -g
else
    CXXFLAGS += -DNDEBUG
endif

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=  -lfilesystem -lfat -lnds9 -lmm9


#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------


ifneq ($(BUILDDIR), $(CURDIR))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(FONTS),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SPRITES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(TILEMAPS),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SOUNDS),$(CURDIR)/$(dir)) \

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#ifneq ($(strip $(NITRODATA)),)
#        export NITRO_FILES := $(CURDIR)/$(NITRODATA)
#endif

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*))) $(SOUNDBANK_NAME).bin
SPRITE_FILES	:=	$(foreach dir,$(FONTS),$(notdir $(wildcard $(dir)/*.png))) \
					$(foreach dir,$(SPRITES),$(notdir $(wildcard $(dir)/*.png)))
TILEMAP_FILES	:=	$(foreach dir,$(TILEMAPS),$(notdir $(wildcard $(dir)/*.png)))
AUDIOFILES	:=	$(foreach dir,$(SOUNDS),$(notdir $(wildcard $(dir)/*)))


#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(BMPFILES:.bmp=.o) \
					$(SPRITE_FILES:.png=.o) \
					$(TILEMAP_FILES:.png=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export AUDIOFILES       :=      $(foreach dir,$(SOUNDS),$(notdir $(wildcard $(dir)/*)))


export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)\

 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) BUILDDIR=`cd $(BUILD) && pwd` --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
 
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds $(TARGET).ds.gba 
 
 
#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds	: 	$(OUTPUT).elf
#$(OUTPUT).nds   :   $(shell find $(TOPDIR)/$(NITRODATA))
$(OUTPUT).elf	:	$(OFILES)
%.s %.h : ../gfx/fonts/%.png
		grit $< -ff../gfx/fonts/font.grit -o$*
%.s %.h : ../gfx/tilemaps/%.png
		grit $< -ff../gfx/tilemaps/tilemap.grit -o$*
%.s %.h : ../gfx/sprites/64x64/%.png
		grit $< -ff../gfx/sprites/64x64/sprite.grit -o$*
%.s %.h : ../gfx/sprites/8x8/%.png
		grit $< -ff../gfx/sprites/8x8/sprite.grit -o$*
%.s %.h : ../gfx/sprites/32x32/%.png
		grit $< -ff../gfx/sprites/32x32/sprite.grit -o$*
%.s %.h : ../gfx/sprites/16x16/%.png
		grit $< -ff../gfx/sprites/16x16/sprite.grit -o$*

#grit $< -ff../gfx/tilemaps/tilemap.grit -o$*
#grit $< -ff../gfx/sprites/sprite.grit -o$*

#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .raw extension
#---------------------------------------------------------------------------------
%.raw.o	:	%.raw
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders 
#---------------------------------------------------------------------------------
%.s %.h	: %.bmp %.grit
#---------------------------------------------------------------------------------
	grit $< -fts -o$*


#---------------------------------------------------------------------------------
# This rule creates the soundbank file for your project using mmutil.
# mmutil takes all audio files in the audio folder and puts them into a
# soundbank file.
#---------------------------------------------------------------------------------
$(SOUNDBANK_NAME).bin : $(AUDIOFILES)
#---------------------------------------------------------------------------------
	@echo $(notdir $^)
	@mmutil -d $^ -o$(SOUNDBANK_NAME).bin -h$(SOUNDBANK_NAME).h



-include $(DEPENDS)
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
