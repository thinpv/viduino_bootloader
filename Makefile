#
# Machine makefile
#

.PHONY:clean
.PHONY:write
.PHONY:mktool
.PHONY:dump
.PHONY:test

BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python
MKSUNXI		:= tools/mksunxi/mksunxi

CROSS_COMPILE = /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC	          = $(CROSS_COMPILE)gcc
CXX	          = $(CROSS_COMPILE)g++
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
AR	          = $(CROSS_COMPILE)ar rcs
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE					= $(CROSS_COMPILE)size

BOOT				= boot
DRIVER			= driver
LIB					= lib
LZMA				= lzma

DEFINES		+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function -D_7ZIP_ST

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T f1c100s.ld -nostdlib
MCFLAGS		:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork

ASFLAGS		+=	-ffreestanding -std=gnu11 $(DEFINES)
CFLAGS		+=	-ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS		+=	-ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti
LDFLAGS		+=	-Wl,-gc-sections 

LIBS 			:= 

SRCDIRS		+= .

INCDIRS		+= $(BOOT)/include $(BOOT)/include/f1c100s
SRCDIRS		+= $(BOOT)

INCDIRS		+= $(LIB) $(LIB)/include
SRCDIRS		+= $(LIB) 
# $(LIB)/libc/string
# $(LIB)/libutils/isoc/arch/arm

INCDIRS		+= $(LZMA) $(LZMA)/file
SRCDIRS		+= $(LZMA) 

# INCDIRS		+= $(DRIVER)/include
# SRCDIRS		+= $(DRIVER)

INCDIRS		+= arm32/include
SRCDIRS		+=

# INCDIRS		+= bootloader/include
# SRCDIRS		+= bootloader

# INCDIRS		+= lzma lzma/file
# SRCDIRS		+= lzma

SFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE	:=	$(patsubst %, -I %, $(INCDIRS))
LIBFILE	:=	$(patsubst %, %, $(LIBFILES))
SOBJS		:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS		:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS		:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))

OBJ = $(SOBJS) $(COBJS) $(CPOBJS)

OBJ_DIRS = $(sort $(dir $(OBJ)))

all: $(BUILD)/firmware.bin

$(OBJ): | $(OBJ_DIRS)
$(OBJ_DIRS):
	$(MKDIR) -p $@

$(BUILD)/%.o: %.S
	$(ECHO) "AS $<"
	$(AS) $(MCFLAGS) $(ASFLAGS) -c $< -o $@
	
$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	$(CC) $(INCLUDE) $(MCFLAGS) $(CFLAGS) -MD -c $< -o $@

$(BUILD)/%.o: %.cpp
	$(ECHO) "CXX $<"
	$(CXX) $(INCLUDE) $(MCFLAGS) $(CPFLAGS) -MD -c $< -o $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -v -O binary $^ $@
	$(ECHO) Make header information for brom booting
	@$(MKSUNXI) $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(CXX) $(LDFLAGS) -Wl,--cref,-Map=$@.map -o $@ $^ $(LIBFILE) $(LIBS)
	$(SIZE) $@

write:
	sudo sunxi-fel -p spiflash-write 0 $(BUILD)/firmware.bin

clean:
	rm -rf $(BUILD)
