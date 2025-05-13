#include <fstream>
#include "config.h"
#include <string>
#include <filesystem>
#include "Logger.h"
#include <iostream>
std::ofstream Logger::logFile;
     void Logger::open() {
        Config* config = Config::getInstance();
        std::string key = config->get("log_file");
        std::filesystem::path filepath(key);
        if(filepath.has_parent_path())
        std::filesystem::create_directories(filepath.parent_path());
        Logger::logFile.open(filepath, std::ios::out | std::ios::app);
    }
    void Logger::log(std::string& message) {
             if (logFile.is_open()) {
            Logger::logFile << message << std::endl;
        } else {
            std::cerr << "Error: Log file is not open." << std::endl;
        }
    }
    void Logger::close() {
        Logger::logFile.close();
    }
