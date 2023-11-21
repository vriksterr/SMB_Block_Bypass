//g++ -o your_program.exe your_program.cpp -static-libgcc -static-libstdc++ -static -lpthread

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <string>
#define SE_SHUTDOWN_NAME L"SeShutdownPrivilege"

int main() {
    // Get the handle of the console window
    HWND hwnd = GetConsoleWindow();

    // Set the window title
    SetWindowTextW(hwnd, L"ISP OP Drive Unblock by Vrikster");
    // Command to execute
    const char* command = "netsh interface portproxy add v4tov4 listenaddress=127.0.0.1 listenport=445 connectaddress=122.176.54.41 connectport=6969";
    // Command to delete: netsh interface portproxy delete v4tov4 listenaddress=127.0.0.1 listenport=445
    // See all the netsh forwards: netsh interface portproxy show all

    // Execute the command with administrator privileges
    HINSTANCE hInstance = ShellExecuteA(NULL, "runas", "cmd.exe", ("/C " + std::string(command)).c_str(), NULL, SW_HIDE);

    // Check the result of the command execution
    if (reinterpret_cast<std::intptr_t>(hInstance) > 32) {
        
        // ANSI escape code for setting text color to lime green
        std::cout << "\x1b[92m";
        // Command executed successfully
        std::cout << "   /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$ \n";
        std::cout << " /$$__  $$| $$  | $$ /$$__  $$ /$$__  $$| $$_____/ /$$__  $$ /$$__  $$\n";
        std::cout << "| $$  \__/| $$  | $$| $$  \__/| $$  \__/| $$      | $$  \__/| $$  \__/\n";
        std::cout << "|  $$$$$$ | $$  | $$| $$      | $$      | $$$$$   |  $$$$$$ |  $$$$$$ \n";
        std::cout << " \____  $$| $$  | $$| $$      | $$      | $$__/    \____  $$ \____  $$\n";
        std::cout << " /$$  \ $$| $$  | $$| $$    $$| $$    $$| $$       /$$  \ $$ /$$  \ $$\n";
        std::cout << "|  $$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$/| $$$$$$$$|  $$$$$$/|  $$$$$$/\n";
        std::cout << " \______/  \______/  \______/  \______/ |________/ \______/  \______/ \n";
        // ANSI escape code for resetting text color to default
        std::cout << "\x1b[0m";

        // Display a message
        std::cout << "Windows Restart is Required for ISP Unblock to Take Effect Press\x1b[38;5;208m\x1b[5m Enter\x1b[0m to restart Windows..." << std::endl;

        // Wait for the user to press Enter
        std::cin.get();

        // Restart Windows
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

        } else {
            // Failed to execute command with administrator privileges
            std::cerr << "Failed to execute with administrator privileges." << std::endl;
        }

        return 0;
    }
