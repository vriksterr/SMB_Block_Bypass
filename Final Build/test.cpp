#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
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
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Port is closed" << std::endl;
    } else {
        std::cout << "Port is open" << std::endl;
        closesocket(sock);
    }

    // Clean up Winsock
    WSACleanup();

    // Pause before closing the console
    system("pause");

    return 0;
}
//test