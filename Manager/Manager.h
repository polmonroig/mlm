/** @file Manager.h
 *  @author Pol Monroig
 *  @brief This is the Manager header, it defines all the
 *         functions that create the interaction with the user
 * */


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
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#define CONFIG_DIR ".mlmconfig"
#define CONFIG_LEN 10

namespace fs = boost::filesystem;



/** @namespace This is the Machine Learning Manager aka "mlm"
 *
 *  @brief This namespace saves all the procedures
 *         used by the mlm application, it also includes some global
 *         functions that are loaded when the program starts
 * */
namespace mlm{


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


    // functions declaration
    void CreateProject(std::string const& name, std::string const& dir, bool default_config);
    std::string JoinPaths(std::string const& p1, std::string const& p2);
    void PushFiles(std::string const& file_type);
    void GenerateConfig();
    void ErrorAndExit(std::string const& error);
    void CreateDir(std::string const& dir);
    void SaveGlobals();
    void LoadGlobals();
    void Configure();
    void WriteHeader(fs::ofstream & file);
    void CreateConfigFile(std::string const& path, std::string const& content="");
    void DisplayVersioning(std::string const& file_type);
    void PushModels();
    void SetModelVersion(int v);
    void AddModel(std::string const& source_path, std::string const& model_name, int model_version);
    std::set<fileType> GetFiles(std::string const& path);
    std::list<fileType> GetInfoFiles(std::string const& path);
    void UpdateInfoFiles(std::set<fileType> const& files, std::list<fileType>& info_files);
    void SaveFilesToInfo(std::list<fileType> const& files, std::string const& path);
    std::string TimetoString(std::time_t t);
    void PullFiles(std::string const &file_type, bool force);
    void PullModels();
    void PullModel(std::string const &model_name, int version);
    std::list<fileType> GetSavedModels(std::string const& path);
    void CopyModel(std::string const& name, int version, std::string const& model_time);



    // global variables
    inline std::string project_name;
    inline std::string project_path;
    inline std::string backup_path;
    inline std::string config;
    inline bool default_config;
    inline bool force_overwrite;

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
