#include "config.h"
#include <map>
#include <cstring>
#include <string>
#include <filesystem>
#include <iostream>

Config* Config::instance = nullptr;
Config::Config() {

    config = std::map<std::string,std::string>();

}

    void Config::default_values() {
        if(config.find("port") == config.end()) {
            config["port"] = "3000";
        } if(config.find("log_file") == config.end()){
        config["log_file"] = "logs/operations.log";
        } if(config.find("data_file") == config.end()){
        config["data_file"] = "data/storage.dat";
        }
        if(config.find("persistence") == config.end()){
            config["persistence"] = "1";
        }
    }
    bool Config::rules(std::string& key,std::string& value) {
        if(key == "port"){
            // value must number
            size_t idx;
            std::stoi(value, &idx);
            errors = "port value must be an integer!";
            return idx == value.size();
        } else if(key == "log_file" || key == "data_file"){
            // value must be a file
            std::filesystem::path path(value);
            if(path.has_filename() == false) {
            errors = "file name entered is an invalid file name!";
            return false;
            }
        } else if(key == "persistence"){
            return value == "0" || value == "1";
        }
        return true;
    }
    bool Config::setup(char** argv, int argc) {
        for(int i = 0; i < argc;i++){
            const char* arg = argv[i];
            // allowed arguments PORT, LOG FILE, DATA_FILE
            // --port=
            // --log_file=
            // --data_file=
            // --persistence=1 or 0 , as in to either write to memory or not by default is persisted
            if(arg[0] != '-' || arg[1] != '-'){
                errors = "format is --<key>=<value> !";
                return false;
            }
            arg += 2;

            char* key = strtok((char *)arg,"=");
            char* value = strtok(NULL,"=");

            // make sure only characters exist in key value
            for(int i = 0; key[i] != '\0'; i++){
                if(!(key[i] >= 'a' && key[i] <= 'z') && !(key[i] >= 'A' && key[i] <= 'Z') && key[i] != '_'){
                    errors = std::string(key) + " is not a proper key!";
                    return false;
                }
            }

            // add key to config if it is a correct key
            std::string cand_key(key);
            if(cand_key == "port" || cand_key == "log_file" || cand_key == "data_file" || cand_key == "persistence") {
                std::string cand_value(value);
            // rules
            if(!rules(cand_key,cand_value)){
            return false;
            }
            config[cand_key] = cand_value;
            }


        }
        // fill items with keys that don't exist yet
        default_values();
        return true;
    }

    std::string Config::get(const std::string& key){
        if(config.find(key) != config.end()) {
            return config[key];
        }
        return "";
    }

    Config* Config::getInstance() {
        if(instance == nullptr) {
            Config::instance = new Config();

        }
        return Config::instance;
    }
    void Config::deleteInstance(){
        if(Config::instance){

            delete Config::instance;
            Config::instance = nullptr;
        }

    }
    std::string Config::getErrors() {
        return errors;
    }
