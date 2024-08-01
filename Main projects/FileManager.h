#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using namespace std;

class FileManager {
private:
    fs::path currentPath;

    uintmax_t calculateDirectorySize(const fs::path& dirPath) const;
    bool confirmAction(const string& action, const string& itemName) const;

public:
    FileManager();

    void listDirectory() const;
    void listFilesWithExtension(const string& extension) const;
    void listHelp() const;
    void changeDirectory(const string& path);
    void displayFileContents(const string& filename) const;
    void createDirectory(const string& dirname);
    void createFile(const string& filename);
    void moveDirectory(const string& src, const string& dst);
    void moveFile(const string& src, const string& dst);
    void renameItem(const string& oldName, const string& newName);
    void deleteItem(const string& path);
    void clearScreen() const;
    fs::path getCurrentPath() const;
};

#endif // FILEMANAGER_H
