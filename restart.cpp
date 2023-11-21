#include <windows.h>

#define SE_SHUTDOWN_NAME L"SeShutdownPrivilege"

int main() {
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

    return 0;
}
