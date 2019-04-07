#ifndef MANAGER_H
#define MANAGER_H

// load std libraries
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

#define CONFIG_DIR ".mlmconfig"

namespace fs = boost::filesystem;



// define manager functions and procedures
namespace mlm{
    // functions declaration
    void CreateProject(std::string const& name, std::string const& dir, bool default_config);
    void UpdateFiles(std::string const& file_type);
    void GenerateConfig();
    void ErrorAndExit(std::string const& error);
    void CreateDir(std::string const& dir);
    void SaveGlobals();
    void LoadGlobals();
    void Configure();
    void WriteHeader(std::ofstream & file);
    void DisplayVersioning(std::string const& file_type);
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

}


#endif
