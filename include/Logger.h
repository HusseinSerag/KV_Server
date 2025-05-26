#pragma once
#include <fstream>
#include "config.h"
#include <string>
#include <filesystem>

class Logger {

    static std::ofstream logFile;
    static bool isEnabled;
    public:
        static void open();
        static void log(std::string& message);
        static void close();


};


