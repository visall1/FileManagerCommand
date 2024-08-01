#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "FileManager.h"
#include <string>

class CommandHandler {
private:
    FileManager fileManager;

public:
    void handleCommand(const std::string& command);
};

#endif // COMMANDHANDLER_H
