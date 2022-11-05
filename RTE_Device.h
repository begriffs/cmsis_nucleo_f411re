#ifndef CUSTOM_RTE_DEVICE_H
#define CUSTOM_RTE_DEVICE_H

/* defaults */

#include "stm32f4xx-dfp-2.16.0/CMSIS/Driver/Config/RTE_Device.h"

/* customizations */

#undef  RTE_USB_OTG_FS
#define RTE_USB_OTG_FS 1

#endif
