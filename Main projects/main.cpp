#include "CommandHandler.h"
#include <iostream>
#include <filesystem>

using namespace std;

void displayWelcomeScreen() {
    FileManager help;
    help.listHelp();
    cout << "path: " << filesystem::current_path() << endl;
}

int main() {
    CommandHandler commandHandler;
    string command;

    displayWelcomeScreen();

    while (true) {
        cout << ">> ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        commandHandler.handleCommand(command);
    }

    return 0;
}
