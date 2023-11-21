#include <iostream>

int main() {
    // ANSI escape codes for text color and blinking
    const char* colorAndBlink = "\x1b[38;5;208m\x1b[5m";
    // ANSI escape code to reset text attributes
    const char* resetAttributes = "\x1b[0m";

    // Box drawing characters
    char horizontalLine = '*';
    char verticalLine = '*';
    char corner = '*';

    // Text to be displayed
    std::string message = "Windows Restart is Required for ISP Unblock to Take Effect Press";

    // Calculate the width of the box
    int boxWidth = message.length() + 4;  // Add 4 for padding on both sides

    // Print the top of the box
    std::cout << corner << std::string(boxWidth, horizontalLine) << corner << std::endl;

    // Print the text inside the box
    std::cout << verticalLine << "  " << message << "  " << verticalLine << std::endl;

    // Print the bottom of the box
    std::cout << corner << std::string(boxWidth, horizontalLine) << corner << std::endl;

    // Print additional instructions
    std::cout << "To restart Windows..." << std::endl;

    // Reset text attributes
    std::cout << resetAttributes;
    std::cin.get();
    return 0;
}
