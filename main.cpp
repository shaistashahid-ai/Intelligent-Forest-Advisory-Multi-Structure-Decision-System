#include <iostream>
#include "console_ui.h"
#include "menu.h"

int main() {
    // Enable ANSI escape codes (required for Windows cmd/PowerShell)
    enableANSI();

    // Print startup splash
    printBanner("IFAMDS - Intelligent Forest Advisory System");
    printInfo("System initializing...");
    printOK("All modules loaded.");
    printDivider();

    Menu::mainMenu();
    return 0;
}
