#include "CommandHandler.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void CommandHandler::handleCommand(const string& command) {
    istringstream iss(command);
    vector<string> tokens;
    string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return;
    }

    const string& cmd = tokens[0];
    if (cmd == "help") {
        fileManager.listHelp();
    } else if (cmd == "ls") {
        if (tokens.size() > 1) {
            fileManager.listFilesWithExtension(tokens[1]);
        } else {
            fileManager.listDirectory();
        }
    } else if (cmd == "cd" && tokens.size() > 1) {
        fileManager.changeDirectory(tokens[1]);
    } else if (cmd == "cat" && tokens.size() > 1) {
        fileManager.displayFileContents(tokens[1]);
    } else if (cmd == "mkdir" && tokens.size() > 1) {
        fileManager.createDirectory(tokens[1]);
    } else if (cmd == "create" && tokens.size() > 1) {
        fileManager.createFile(tokens[1]);
    } else if (cmd == "mvdir" && tokens.size() > 2) {
        fileManager.moveDirectory(tokens[1], tokens[2]);
    } else if (cmd == "mvfile" && tokens.size() > 2) {
        fileManager.moveFile(tokens[1], tokens[2]);
    } else if (cmd == "rename" && tokens.size() > 2) {
        fileManager.renameItem(tokens[1], tokens[2]);
    } else if (cmd == "delete" && tokens.size() > 1) {
        fileManager.deleteItem(tokens[1]);
    } else if (cmd == "clear") {
        fileManager.clearScreen();
    } else {
        cout << "Unknown command: " << cmd << "\n";
    }
}
