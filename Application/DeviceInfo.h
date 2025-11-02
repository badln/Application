#ifndef DEVICEINFO
#define DEVICEINFO

#include <intrin.h>
#include <array>
#include "main.h"
#include "psapi.h"

class DeviceInfo {
public:

    static MEMORYSTATUSEX MemoryStatus;
    static SYSTEM_INFO SystemInfo;

    static std::string CPUName();
    static std::string CPUSpeed();
};

#endif

