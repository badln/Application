#include "DeviceInfo.h"

MEMORYSTATUSEX DeviceInfo::MemoryStatus;
SYSTEM_INFO DeviceInfo::SystemInfo;
std::string DeviceInfo::CPUName()
{
    std::array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

    std::array<char, 64> charBuffer = {};

    constexpr std::array<int, 3> functionIds = {
        0x8000'0002,
        0x8000'0003,
        0x8000'0004
    };

    std::string cpu;

    for (int id : functionIds)
    {
        __cpuid(integerBuffer.data(), id);
        std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
        cpu += std::string(charBuffer.data());
    }

    return cpu;
}

std::string DeviceInfo::CPUSpeed()
{
    return 0; // this approach doesn't work
    wchar_t Buffer[_MAX_PATH];
    DWORD BufSize = _MAX_PATH;
    DWORD dwMHz = _MAX_PATH;
    HKEY hKey;

    // open the key where the proc speed is hidden:
    long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        reinterpret_cast<LPCSTR>(L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"),
        0,
        KEY_READ,
        &hKey);
    if (lError != ERROR_SUCCESS)
    {// if the key is not found, tell the user why:
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            lError,
            0,
            reinterpret_cast<LPSTR>(Buffer),
            _MAX_PATH,
            0);
        wprintf(Buffer);
        return 0;
    }

    // query the key:
    RegQueryValueEx(hKey, reinterpret_cast<LPCSTR>(L"~MHz"), NULL, NULL, (LPBYTE)&dwMHz, &BufSize);
    return std::to_string(dwMHz);
}