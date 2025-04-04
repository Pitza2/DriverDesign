#include <devguid.h>
#include <iostream>
#include <string>
using namespace std;

#pragma comment(lib, "setupapi.lib")

void EnumerateDevices(const GUID* guid, const char* category) {
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(guid, 0, 0, DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        cout << "Error getting " << category << " devices" << endl;
        return;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    cout << "\n" << category << " devices:\n";

    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        char deviceName[256] = { 0 };
        char hardwareID[1024] = { 0 };

        if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)deviceName, sizeof(deviceName), NULL)) {
            cout << "Device: " << deviceName;
        }

        // Get hardware ID for more info
        if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)hardwareID, sizeof(hardwareID), NULL)) {
            cout << " (Hardware ID: " << hardwareID << ")";
        }
        cout << endl;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

int main() {
    // Check multiple device classes
    EnumerateDevices(&GUID_DEVCLASS_USB, "USB");
    EnumerateDevices(&GUID_DEVCLASS_HIDCLASS, "HID");
    EnumerateDevices(&GUID_DEVCLASS_MOUSE, "Mouse");
    EnumerateDevices(&GUID_DEVCLASS_KEYBOARD, "Keyboard");

    return 0;
}