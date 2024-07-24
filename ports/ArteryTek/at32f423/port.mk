UF2_FAMILY_ID ?= 0x7f74372d
CROSS_COMPILE = arm-none-eabi-

AT_DRIVERS = sdk/ArteryTek/at32f423_library/libraries/drivers
AT_CMSIS = sdk/ArteryTek/at32f423_library/libraries/cmsis

# Port Compiler Flags
CFLAGS += \
  -flto \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m4 \
  -mfloat-abi=hard \
  -mfpu=fpv4-sp-d16 \
  -nostdlib -nostartfiles

# suppress warning caused by vendor mcu driver
CFLAGS += -Wno-error=cast-align -Wno-error=unused-parameter 

# default linker file
LD_FILES ?= $(PORT_DIR)/ld/AT32F423xC_FLASH.ld

# Port source
SRC_C += \
	$(addprefix $(CURRENT_PATH)/, $(wildcard *.c)) \
	$(AT_CMSIS)/cm4/device_support/system_at32f423.c \
	$(AT_DRIVERS)/src/at32f423_crm.c \
	$(AT_DRIVERS)/src/at32f423_flash.c \
	$(AT_DRIVERS)/src/at32f423_gpio.c \
	$(AT_DRIVERS)/src/at32f423_misc.c \
	$(AT_DRIVERS)/src/at32f423_pwc.c 

# Port include
INC += \
	$(TOP)/$(AT_CMSIS)/cm4/core_support \
	$(TOP)/$(AT_CMSIS)/cm4/device_support \
	$(TOP)/$(AT_DRIVERS)/inc

# Port source for USB
SRC_C += \
	$(CHERRYUSB_DIR)/port/dwc2/usb_dc_dwc2.c \
	$(CHERRYUSB_DIR)/port/dwc2/usb_glue_at.c

# Port include for USB port
INC += $(TOP)/$(CHERRYUSB_DIR)/port/dwc2

DEPFLAGS += -include $(TOP)/$(AT_CMSIS)/cm4/device_support/at32f423.h