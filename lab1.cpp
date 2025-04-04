#include <windows.h>
#include <iostream>
#include <setupapi.h>
#include <devpkey.h>
#include <winusb.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "kernel32.lib")

void ListUSBDevices()
{
    // Retrieve a handle to the device information set for all connected devices of USB class
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB,    // USB devices class GUID
        nullptr,               // All devices
        nullptr,               // Parent window
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get device information set." << std::endl;
        return;
    }

    // Enumerate through the devices
    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        // Get the device description (device name)
        char deviceName[256];
        if (SetupDiGetDeviceRegistryProperty(
                deviceInfoSet, 
                &deviceInfoData, 
                SPDRP_DEVICEDESC, 
                nullptr, 
                (PBYTE)deviceName, 
                sizeof(deviceName), 
                nullptr)) {
            std::cout << "Device " << i + 1 << ": " << deviceName << std::endl;
        }
    }

    // Cleanup
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

int main()
{
    ListUSBDevices();
    return 0;
}
