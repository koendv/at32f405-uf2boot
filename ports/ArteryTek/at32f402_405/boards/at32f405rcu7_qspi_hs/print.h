#ifndef _PRINT_RTT_H_
#define _PRINT_RTT_H_
#include "SEGGER_RTT.h"
#define LOG(format, args...)  SEGGER_RTT_printf(0, "[%s:%d] "format, __FILE__, __LINE__, ##args)
//#define printf(format, args...)  SEGGER_RTT_printf(0, format, ##args)
//#define printf(format, args...)
#endif

