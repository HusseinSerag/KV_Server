#pragma once
#include <map>
#include <cstring>
#include <string>
#include <filesystem>

class Config {

    std::map<std::string,std::string> config;
    std::string errors;
    static Config* instance;
    void default_values();
    bool rules(std::string& key,std::string& value);
    Config();
    public:
    bool setup(char** argv, int argc);
    std::string get(const std::string& key);
    static Config* getInstance();
    static void deleteInstance();
    std::string getErrors();


};
