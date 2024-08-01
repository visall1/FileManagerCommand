#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

class FileManager {
private:
    fs::path currentPath;

    uintmax_t calculateDirectorySize(const fs::path& dirPath) const {
        uintmax_t size = 0;
        for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
            if (fs::is_regular_file(entry.status())) {
                size += fs::file_size(entry.path());
            }
        }
        return size;
    }

    bool confirmAction(const string& action, const string& itemName) const {
        string confirmation;
        cout << "Are you sure you want to " << action << " '" << itemName << "'? (y/n): ";
        cin >> confirmation;
        return confirmation == "y" || confirmation == "Y";
    }

public:
    FileManager() : currentPath(fs::current_path()) {}

    void listDirectory() const {
        const int nameWidth = 30;
        const int sizeWidth = 15;
        cout << "Listing directory: " << currentPath << "\n";
        cout << left << setw(nameWidth) << "Name" << right << setw(sizeWidth) << "Size" << "\n";
        cout << string(nameWidth + sizeWidth, '-') << "\n";
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (entry.is_directory()) {
                uintmax_t dirSize = calculateDirectorySize(entry.path());
                cout << left << setw(nameWidth) << "[DIR] " + entry.path().filename().string() << right << setw(sizeWidth) << dirSize << " bytes\n";
            } else if (entry.is_regular_file()) {
                cout << left << setw(nameWidth) << "[FILE] " + entry.path().filename().string() << right << setw(sizeWidth) << fs::file_size(entry.path()) << " bytes\n";
            }
        }
    }

    void listFilesWithExtension(const string& extension) const {
        cout << "Listing files with extension " << extension << " in directory: " << currentPath << "\n";
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                cout << entry.path().filename().string() << "\n";
            }
        }
    }

    void listHelp() const {
        cout << "--------------------------------------------------------------\n"
             << "\t\t\t File Manager \n"
             << "--------------------------------------------------------------\n";
        cout << "Help:\n"
             << " - help                : Display this help message\n"
             << " - ls                  : List directory contents\n"
             << " - ls <ext>            : List files with a specific extension\n"
             << " - cd <dir>            : Change directory\n"
             << " - cat <file>          : Display file contents\n"
             << " - mkdir <dir>         : Create a new directory\n"
             << " - create <file>       : Create a new .txt file\n"
             << " - mvdir <src> <dst>   : Move directory\n"
             << " - mvfile <src> <dst>  : Move file\n"
             << " - rename <src> <dst>  : Rename file or directory\n"
             << " - delete <path>       : Delete file or directory\n"
             << " - clear               : Clear the screen\n"
             << " - exit                : Exit the file manager\n";
    }

    void changeDirectory(const string& path) {
        fs::path newPath = currentPath / path;
        if (fs::exists(newPath) && fs::is_directory(newPath)) {
            currentPath = fs::canonical(newPath);
            cout << "Changed directory to: " << currentPath << "\n";
        } else {
            cout << "Invalid directory: " << path << "\n";
        }
    }

    void displayFileContents(const string& filename) const {
        fs::path filePath = currentPath / filename;
        if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
            ifstream file(filePath);
            string line;
            while (getline(file, line)) {
                cout << line << "\n";
            }
        } else {
            cout << "File not found: " << filename << "\n";
        }
    }

    void createDirectory(const string& dirname) {
        fs::path dirPath = currentPath / dirname;
        if (!fs::exists(dirPath)) {
            fs::create_directory(dirPath);
            cout << "Directory created: " << dirPath << "\n";
        } else {
            cout << "Directory already exists: " << dirPath << "\n";
        }
    }

    void createFile(const string& filename) {
        fs::path filePath = currentPath / filename;
        if (!fs::exists(filePath)) {
            ofstream file(filePath);
            cout << "File created: " << filePath << "\n";
        } else {
            cout << "File already exists: " << filePath << "\n";
        }
    }

    void moveDirectory(const string& src, const string& dst) {
        fs::path srcPath = currentPath / src;
        fs::path dstPath = currentPath / dst;
        if (fs::exists(srcPath) && fs::is_directory(srcPath)) {
            fs::rename(srcPath, dstPath);
            cout << "Directory moved from " << srcPath << " to " << dstPath << "\n";
        } else {
            cout << "Invalid source directory: " << src << "\n";
        }
    }

    void moveFile(const string& src, const string& dst) {
        fs::path srcPath = currentPath / src;
        fs::path dstPath = currentPath / dst;
        if (fs::exists(srcPath) && fs::is_regular_file(srcPath)) {
            fs::rename(srcPath, dstPath);
            cout << "File moved from " << srcPath << " to " << dstPath << "\n";
        } else {
            cout << "Invalid source file: " << src << "\n";
        }
    }

    void renameItem(const string& oldName, const string& newName) {
        fs::path oldPath = currentPath / oldName;
        fs::path newPath = currentPath / newName;
        if (fs::exists(oldPath)) {
            if (confirmAction("rename", oldName)) {
                fs::rename(oldPath, newPath);
                cout << "Renamed '" << oldPath << "' to '" << newPath << "'\n";
            } else {
                cout << "Rename operation canceled.\n";
            }
        } else {
            cout << "File or directory not found: " << oldName << "\n";
        }
    }

    void deleteItem(const string& path) {
        fs::path itemPath = currentPath / path;
        if (fs::exists(itemPath)) {
            if (confirmAction("delete", path)) {
                if (fs::is_directory(itemPath)) {
                    fs::remove_all(itemPath);
                    cout << "Directory deleted: " << itemPath << "\n";
                } else {
                    fs::remove(itemPath);
                    cout << "File deleted: " << itemPath << "\n";
                }
            } else {
                cout << "Delete operation canceled.\n";
            }
        } else {
            cout << "File or directory not found: " << path << "\n";
        }
    }

    void clearScreen() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    fs::path getCurrentPath() const {
        return currentPath;
    }
};

class CommandHandler {
private:
    FileManager fileManager;

public:
    void handleCommand(const string& command) {
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
};

void displayWelcomeScreen() {
    FileManager Help;
    Help.listHelp();
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
