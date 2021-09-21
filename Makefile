#
# Machine makefile
#

.PHONY:clean
.PHONY:write write2

VERSION			= 1.0.0

BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python
MKSUNXI			:= tools/mksunxi/mksunxi
MKZ					:= tools/mkz/mkz
XFEL				:= tools/xfel/xfel
ENCRYPT_KEY	:= "679408dc82ae80d411d5d9720b65a43fc4f1534fa563fb28c6cd8928e46aaae9"
PUBLIC_KEY	:= "03cfd18e4a4b40d6529448aa2df8bbb677128258b8fbfc5b9e492fbbba4e84832f"
PRIVATE_KEY	:= "dc57b8a9e0e2b7f8b4c929bd8db2844e53f01f171bbcdf6e628908dbf2b2e6a9"
MESSAGE			:= "https://thinpv.github.io/"

CROSS_COMPILE = /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC	          = $(CROSS_COMPILE)gcc
CXX	          = $(CROSS_COMPILE)g++
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
AR	          = $(CROSS_COMPILE)ar rcs
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE					= $(CROSS_COMPILE)size

BOOT			= boot
DRIVER		= driver
LIB				= lib
LZMA			= lzma
LOADER		= loader

DEFINES		+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function -D_7ZIP_ST -DVERSION=\"$(VERSION)\"

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
CPFLAGS		:= -g -ggdb -Wall -O3
LDFLAGS		:= -T f1c100s.ld -nostdlib
MCFLAGS		:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork

ASFLAGS		+= -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS		+= -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS		+= -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti
LDFLAGS		+= -Wl,-gc-sections 

CFLAGS		+= -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-value 

LIBS 			:= -lgcc -lm -lc -lnosys

SRCDIRS		+= .

INCDIRS		+= $(BOOT)/include $(BOOT)/include/f1c100s
SRCDIRS		+= $(BOOT)

INCDIRS		+= $(LIB)
SRCDIRS		+= $(LIB)

INCDIRS		+= $(DRIVER)/include
SRCDIRS		+= $(DRIVER)

INCDIRS		+= $(LOADER)/include
SRCDIRS		+= $(LOADER)

INCDIRS		+= $(LZMA)
SRCDIRS		+= $(LZMA) 

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
	@$(MKDIR) -p $@

$(BUILD)/%.o: %.S
	$(ECHO) "AS $<"
	@$(AS) $(MCFLAGS) $(ASFLAGS) -c $< -o $@
	
$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	@$(CC) $(INCLUDE) $(MCFLAGS) $(CFLAGS) -MD -c $< -o $@

$(BUILD)/%.o: %.cpp
	$(ECHO) "CXX $<"
	@$(CXX) $(INCLUDE) $(MCFLAGS) $(CPFLAGS) -MD -c $< -o $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -v -O binary $^ $@
	$(ECHO) Make header information for brom booting
	@$(MKSUNXI) $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	@$(CXX) $(LDFLAGS) -Wl,--cref,-Map=$@.map -o $@ $^ $(LIBFILE) $(LIBS)
	@$(SIZE) $@

write:
	@# sudo sunxi-fel -p spiflash-write 0 $(BUILD)/firmware.bin
	@sudo $(XFEL) spinor write 0 $(BUILD)/firmware.bin
	@sudo $(XFEL) reset

write2:
	@$(eval UNIQUEID=$(shell sudo $(XFEL) sid))
	@$(ECHO) Device $(UNIQUEID) write version $(VERSION)
	@sudo $(MKZ) -majoy 3 -minior 0 -patch 0 -r 24576 -k $(ENCRYPT_KEY) -pb $(PUBLIC_KEY) -pv $(PRIVATE_KEY) -m $(MESSAGE) -g $(UNIQUEID) -i $(UNIQUEID) $(BUILD)/firmware.bin $(BUILD)/firmware.bin.z
	@sudo $(XFEL) spinor write 0 $(BUILD)/firmware.bin.z
	# sudo sunxi-fel -p spiflash-write 0 $(BUILD)/firmware.bin.z
	@sudo $(XFEL) reset
	@$(ECHO) Update info to google sheet
	@python3 tools/ggsheet/pushtogoogle.py $(UNIQUEID) $(VERSION)
	@$(ECHO) DONE

clean:
	rm -rf $(BUILD)
