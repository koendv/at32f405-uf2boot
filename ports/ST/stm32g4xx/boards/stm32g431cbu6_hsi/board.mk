CFLAGS += \
  -DSTM32G431xx \
  -DHSE_VALUE=8000000U \

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32g431xx.s
