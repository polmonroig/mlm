#ifndef MANAGER_H
#define MANAGER_H

// load std libraries
#include <string>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

#define CONFIG_DIR ".mlmconfig"

namespace fs = boost::filesystem;



// define manager functions and procedures
namespace mlm{
    // functions declaration
    void CreateProject(std::string const& name, std::string const& dir, bool default_config);
    void AddFile(std::string const& file_path);
    void GenerateConfig();
    void ErrorAndExit(std::string const& error);
    void CreateDir(std::string const& dir);
    void SaveGlobals();
    void LoadGlobals();
    void Configure();
    std::string JoinPaths(std::string const& p1, std::string const& p2);


    // global variables
    inline std::string project_name;
    inline std::string project_path;
    inline std::string config;
    inline bool default_config;

}


#endif
