#include <Windows.h>
#include <winreg.h>
#include <stdio.h>
#include <iostream>


void PrintImagePathFromRegistry(const char* path) {
    HKEY hKey;
    LONG lResult;

    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey);
    if (lResult != ERROR_SUCCESS) {
        std::cerr << "Failed to open registry key. Error code: " << lResult << std::endl;
        return;
    }

    // Enumerate all subkeys
    DWORD dwIndex = 0;
    char SubKey[256];
    DWORD dwSize = sizeof(SubKey);

    while ((lResult = RegEnumKeyExA(hKey, dwIndex, SubKey, &dwSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS) {
        // For each subkey, open it and query the "ImagePath" value
        HKEY hSubKey;
        lResult = RegOpenKeyExA(hKey, SubKey, 0, KEY_READ, &hSubKey);
        if (lResult == ERROR_SUCCESS) {
            char szImagePath[1024];
            DWORD dwImagePathSize = sizeof(szImagePath);
            lResult = RegQueryValueExA(hSubKey, "ImagePath", NULL, NULL, (LPBYTE)szImagePath, &dwImagePathSize);

            if (lResult == ERROR_SUCCESS) {
                std::cout << "Subkey: " << SubKey << std::endl;
                std::cout << "ImagePath: " << szImagePath << std::endl;
            }
            else if (lResult == ERROR_FILE_NOT_FOUND) {
                std::cout << "Subkey: " << SubKey << " does not have an ImagePath value." << std::endl;
            }
            else {
                std::cout << "Failed to query ImagePath for subkey: " << SubKey << ". Error code: " << lResult << std::endl;
            }

            RegCloseKey(hSubKey);
        }
        else {
            std::cout << "Failed to open subkey: " << SubKey << ". Error code: " << lResult << std::endl;
        }

        dwSize = sizeof(SubKey);
        dwIndex++;
    }

    if (lResult != ERROR_NO_MORE_ITEMS) {
        std::cout << "Error enumerating subkeys. Error code: " << lResult << std::endl;
    }

    RegCloseKey(hKey);
}

int main() {
    PrintImagePathFromRegistry("SYSTEM\\CurrentControlSet\\Services");
    return 0;
}
