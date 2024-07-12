CFLAGS += \
  -DAT32F405CCU7 \
  -Wno-discarded-qualifiers \
  -Wno-cast-align \
  -Wno-unused-value \
  -Wno-unused-variable \
  -Wno-unused-but-set-variable \
  -Wno-unused-parameter \
  -Wno-suggest-attribute=format \
  -Wno-type-limits \
  -DHEXT_VALUE=12000000U \
  -DCONFIG_USB_HS \
  -DCONFIG_DWC2_VBUS_SENSING=0 \
  -DCFG_UF2_SECTORS_PER_CLUSTER=4 \
  -DSystemCoreClock=system_core_clock

SRC_S += \
	$(AT_CMSIS)/cm4/device_support/startup/gcc/startup_at32f402_405.s

# sfud
INC += \
  $(TOP)/lib/SFUD/sfud/inc/

SRC_C += \
  $(BOARD_DIR)/qspi1_init.c \
  $(BOARD_DIR)/qspi1_send_receive.c \
  $(BOARD_DIR)/qspi1_xip.c \
  $(BOARD_DIR)/at32f402_405_board.c \
  $(BOARD_DIR)/board.c \
  $(BOARD_DIR)/sfud_port.c \
  $(BOARD_DIR)/wk_config.c \
  $(TOP)/lib/SFUD/sfud/src/sfud.c \
  $(TOP)/lib/SFUD/sfud/src/sfud_sfdp.c

# segger rtt
INC += \
	$(TOP)/lib/SEGGER_RTT/RTT

SRC_C += \
	lib/SEGGER_RTT/RTT/SEGGER_RTT.c \
	lib/SEGGER_RTT/RTT/SEGGER_RTT_printf.c

USB_XFER_SPEED = HS
