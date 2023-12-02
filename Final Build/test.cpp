//g++ -o your_program.exe your_program.cpp -static-libgcc -static-libstdc++ -static -lpthread -lws2_32

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <winsock2.h>
#include <shellapi.h>
#include <string>
#define SE_SHUTDOWN_NAME L"SeShutdownPrivilege"

//Clears the Output of the Screen
void clear_screen(char fill = ' ') { 
    COORD tl = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);   
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

int restart_windows(){
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
}

int isp_unblock_bypass(){
        // Command to execute
    const char* command = "netsh interface portproxy add v4tov4 listenaddress=127.0.0.1 listenport=445 connectaddress=122.176.54.41 connectport=6969";
    // Command to delete: netsh interface portproxy delete v4tov4 listenaddress=127.0.0.1 listenport=445
    // See all the netsh forwards: netsh interface portproxy show all
    // Reset netsh interface portproxy: netsh interface portproxy reset

    // Execute the command with administrator privileges
    HINSTANCE hInstance = ShellExecuteA(NULL, "runas", "powershell.exe", ("/C " + std::string(command)).c_str(), NULL, SW_HIDE);

    // Check the result of the command execution
    if (reinterpret_cast<std::intptr_t>(hInstance) > 32) {
        
        // ANSI escape code for setting text color to lime green
        std::cout << "\x1b[92m";
        // Command executed successfully
        std::cout << "   /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$ \n";
        std::cout << " /$$__  $$| $$  | $$ /$$__  $$ /$$__  $$| $$_____/ /$$__  $$ /$$__  $$ \n";
        std::cout << "| $$  \__/| $$  | $$| $$  \__/| $$  \__/| $$      | $$  \__/| $$  \__/ \n";
        std::cout << "|  $$$$$$ | $$  | $$| $$      | $$      | $$$$$   |  $$$$$$ |  $$$$$$  \n";
        std::cout << " \____  $$| $$  | $$| $$      | $$      | $$__/    \____  $$ \____  $$ \n";
        std::cout << " /$$  \ $$| $$  | $$| $$    $$| $$    $$| $$       /$$  \ $$ /$$  \ $$ \n";
        std::cout << "|  $$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$/| $$$$$$$$|  $$$$$$/|  $$$$$$/ \n";
        std::cout << " \______/  \______/  \______/  \______/ |________/ \______/  \______/  \n";
        // ANSI escape code for resetting text color to default
        std::cout << "\x1b[0m";

        // Display a message
        // ANSI escape codes for text color and blinking
        const char* colorAndBlink = "\x1b[38;5;208m\x1b[5m";
        // ANSI escape code to reset text attributes
        const char* resetAttributes = "\x1b[0m";

        // Box drawing characters
        char horizontalLine = '*';
        char verticalLine = '*';
        char corner = '*';

        // Text to be displayed
        std::string message = "Windows Restart is Required for ISP Unblock to Take Effect Press\x1b[38;5;208m\x1b[5m Enter\x1b[0m to restart Windows...";

        // Calculate the width of the box
        int boxWidth = message.length() - 15;  // Add 4 for padding on both sides

        // Print the top of the box
        std::cout << corner << std::string(boxWidth, horizontalLine) << corner << std::endl;

        // Print the text inside the box
        std::cout << verticalLine << "  " << message << "  " << verticalLine << std::endl;

        // Print the bottom of the box
        std::cout << corner << std::string(boxWidth, horizontalLine) << corner << std::endl;

        // Reset text attributes
        std::cout << resetAttributes;

        // Wait for the user to press Enter
        std::cin.get();

        restart_windows();
        } else {
            // Failed to execute command with administrator privileges
            std::cerr << "Failed to execute with administrator privileges." << std::endl;
        }
}
int reset_everthing(){
    // Build the command to run netsh with admin privileges
    const char* command = "cmd.exe /C runas /user:Administrator \"netsh interface portproxy reset\"";

    // Execute the command
    int result = system(command);

    // Check the result of the command execution
    if (result == 0) {
        // The command executed successfully
        std::cout << "netsh command executed successfully." << std::endl;
        restart_windows();
    } else {
        // There was an error executing the command
        std::cerr << "Error executing netsh command. Error code: " << result << std::endl;
    }
}

int drive_mount(){
    // Get the drive letter from the user
    std::cout << "Enter the drive letter you want to use for opDrive(Choose unique letter you are not currently using): ";
    char driveLetter;
    std::cin >> driveLetter;

    // Convert drive letter to uppercase
    driveLetter = std::toupper(driveLetter);

    // Form the command string
    std::string command = "net use ";
    command += driveLetter;
    command += ": \\\\127.0.0.1\\opdrive";

    // Execute the command
    int result = system(command.c_str());

    // Check if the command was executed successfully
    if (result == 0) {
        std::cout << "Drive Mount successful." << std::endl << std::endl;
        std::cout << "Going Back to Main Menu..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
        
        return true;

    } else {
        std::cerr << "Error executing." << std::endl;
        std::cout << "Going Back to Main Menu..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";
                Sleep(1000);
        std::cout << "..";

        return false;
    }
}

int isp_block_check(){
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Get user input for IP address and port
    // std::cout << "Enter IP address: ";
    std::string ipAddress = "122.176.54.41";
    // std::cin >> ipAddress;

    // std::cout << "Enter port number: ";
    int port=445;
    // std::cin >> port;

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up sockaddr_in structure
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    // Connect to server
    if (connect(sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Your ISP is Blocking Communication! Applying Bypass" << std::endl;
        isp_unblock_bypass();
        drive_mount();
    } else {
        std::cout << "Your ISP is Allowing Communication! No Bypass Required Mount your Drive" << std::endl;
        closesocket(sock);
        drive_mount();
    }
}

int main() {
    
     // Get the handle of the console window
    HWND hwnd = GetConsoleWindow();
    // Set the window title
    SetWindowTextW(hwnd, L"OP Drive Tool by Vrikster");

    while (true){
        std::cout<<"Welcome to the OP Drive Tool Choose from Below Options"<<std::endl;
        std::cout<<"******************************************************"<<std::endl;
        std::cout<<"1. Mount OP Drive on your PC"<<std::endl;
        std::cout<<"2. Revert back all changes made to this pc by the tool (this will also unmount/remove Drive)"<<std::endl;

        int selection_option_number{};
        std::cout<<"Choose your option: ";
        std::cin>> selection_option_number;

        if(selection_option_number == 1){
            clear_screen();
            isp_block_check();
        }
        else if(selection_option_number == 2){
            reset_everthing();
        }
        else{
            std::cout<<"Wrong Option chosen"<<std::endl;
        }
    std::cin.get(); // Wait for a key press before displaying the menu again
    clear_screen(); // Clear the screen before displaying the menu again
    }
    return 0;
}
