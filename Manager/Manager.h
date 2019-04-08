#ifndef MANAGER_H
#define MANAGER_H

// load std libraries
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <utility>
#include <ctime>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#define CONFIG_DIR ".mlmconfig"
#define CONFIG_LEN 10

namespace fs = boost::filesystem;



// define manager functions and procedures
namespace mlm{
    // functions declaration
    void CreateProject(std::string const& name, std::string const& dir, bool default_config);
    void PushFiles(std::string const& file_type);
    void GenerateConfig();
    void ErrorAndExit(std::string const& error);
    void CreateDir(std::string const& dir);
    void SaveGlobals();
    void LoadGlobals();
    void Configure();
    void WriteHeader(std::ofstream & file);
    void CreateConfigFile(std::string const& path, std::string const& content="");
    void DisplayVersioning(std::string const& file_type);
    void PushModels();
    void SetModelVersion(int v);
    std::string TimetoString(std::time_t t);
    std::string JoinPaths(std::string const& p1, std::string const& p2);



    // global variables
    inline std::string project_name;
    inline std::string project_path;
    inline std::string config;
    inline bool default_config;

    // constants
    std::string const MODELS = "models";
    std::string const DATA = "data";
    std::string const ALL = "all";
    std::string const MODELS_DIR = "models/";
    std::string const DATA_DIR = "data/";
    std::string const TRAIN_DIR = "data/train/";
    std::string const TEST_DIR = "data/test/";


    // types

    struct fileType{
        // variables
        std::string name;
        std::string time;
        int version;

        // constructor
        fileType(std::string const& s, std::string const& t, int v){
            name = s;
            time = t;
            version = v;
        }

        void SetVersion(int v){
            version = v;
        }


        // operator
        bool operator<(const fileType& file) const {
            if(name != file.name)return name < file.name;
            else return time < file.time;
        }

        bool operator>(const fileType& file) const {
            if(name != file.name)return name > file.name;
            else return time > file.time;
        }

        bool operator==(const fileType& file) const {
            return name == file.name && file.time == time;
        }
    };


}


#endif
