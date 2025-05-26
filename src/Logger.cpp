#include <fstream>
#include "config.h"
#include <string>
#include <filesystem>
#include "Logger.h"
#include <iostream>
std::ofstream Logger::logFile;
bool Logger::isEnabled = false;
     void Logger::open() {
        Config* config = Config::getInstance();
        const std::string& enabled =  config->get("log_enabled");
        Logger::isEnabled = enabled == "1";
        if(Logger::isEnabled){

            const std::string& key = config->get("log_file");
            std::filesystem::path filepath(key);
            if(filepath.has_parent_path())
            std::filesystem::create_directories(filepath.parent_path());
            Logger::logFile.open(filepath, std::ios::out | std::ios::app);
        }
    }
    void Logger::log(std::string& message) {
        
        
        
            if(Logger::isEnabled){

                
                if (logFile.is_open()) {
                    Logger::logFile << message << std::endl;
                } else {
                    std::cerr << "Error: Log file is not open." << std::endl;
                }
            }
        
    }
    void Logger::close() {
        if(Logger::isEnabled){
            Logger::logFile.close();
        }
    }
