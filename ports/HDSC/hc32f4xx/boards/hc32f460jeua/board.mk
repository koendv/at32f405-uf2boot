CFLAGS += \
  -DHC32F460 \
  -DXTAL_VALUE=8000000U \
  -DSTM32F7

SRC_S += \
	$(HC_CMSIS)/Device/HDSC/hc32f4xx/Source/GCC/startup_hc32f460.S
