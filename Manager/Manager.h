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



/** @namespace mlm This is the Machine Learning Manager aka "mlm"
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

    /** @brief Create a new project folder and its configuration
     *  @param name: name of the project
     *  @param dir: project directory
     *  @param default_config: use default configuration or manual
     * */
    void CreateProject(std::string const& name, std::string const& dir, bool default_config);

    /** @brief Join two paths
     *  @param p1: path 1
     *  @param p2: path 2
     *  @return returns a string with the path resulting from the join of p1 an p2
     * */
    std::string JoinPaths(std::string const& p1, std::string const& p2);

    /** @brief Push files of a specific type to the backup dir
     *  @param file_type: file type to push
     * */
    void PushFiles(std::string const& file_type);

    /** @brief Generates the basic configuration files and folders for
     *         a new project based on the current configuration
     * */
    void GenerateConfig();

    /** @brief Prints an error message to screen and exits the program
     *  @param error: message to print
     * */
    void ErrorAndExit(std::string const& error);

    /** @brief Create a new folder
     *  @param dir: directory to create
     * */
    void CreateDir(std::string const& dir);

    /** @brief Saves the basic project variables to CONFIG_DIR/info
     * */
    void SaveGlobals();

    /** @brief Loads the basic project variables
     * */
    void LoadGlobals();

    /** @brief Sets some parameters of the project configuration
     * */
    void Configure();

    /** @brief Writes a header to a file
     *  @param file: file to write
     * */
    void WriteHeader(fs::ofstream & file);

    /** @brief Creates a configuration file with an additional content
     *  @param path: path for the new configuration file
     *  @param content: content to append to the file
     * */
    void CreateConfigFile(std::string const& path, std::string const& content="");

    /** @brief Displays the files of a specific type with their
     *         last modification date and their version
     *  @param file_type: type of the file to print
     * */
    void DisplayVersioning(std::string const& file_type);

    /** @brief Pushes all the models from the project directory to the
     *         backup directory
     * */
    void PushModels();

    /** @brief Sets the latest model a all the models to the project configuration
     *  @param v: version to compare
     * */
    void SetModelVersion(int v);

    /** @brief Moves a model from a specific path to the backup based on its version
     *  @param source_path: the source path of the model including its name
     *  @param model_name: name of the model
     *  @param model_version: version of the model
     * */
    void AddModel(std::string const& source_path, std::string const& model_name, int model_version);

    /** @brief Collects all the files of a specific directory
     *  @param path: path of the files to collect
     *  @return set of files
     * */
    std::set<fileType> GetFiles(std::string const& path);

    /** @brief Collects all the files from a info file
     *  @param path: path to the info file
     *  @return list of files
     * */
    std::list<fileType> GetInfoFiles(std::string const& path);

    /** @brief Adds files from a location to the backup directory,
     *         thus updating the backup models
     *  @param files: set of files to transfer
     *  @param info_files: list of files already in backup
     * */
    void UpdateInfoFiles(std::set<fileType> const& files, std::list<fileType>& info_files);

    /** @brief Saves a list fo files to an info file
     *  @param files: files to save
     *  @param path: location of the info file
     * */
    void SaveFilesToInfo(std::list<fileType> const& files, std::string const& path);

    /** @brief Converts a time variable to a string
     *  @param t: time to convert
     *  @return string of the t
     * */
    std::string TimeToString(std::time_t t);

    /** @brief Copies files of a specific type from backup to the project
     *  @param file_type: type of the file to move
     *  @param force: true to force overwrite files with the same name
     * */
    void PullFiles(std::string const &file_type, bool force);

    /** @brief Copies the models from backup to the project directory
     * */
    void PullModels();

    /** @brief Copies a specific model with a specific version
     *         from backup to the project directory;
     *         if version == -1 then copy latest version
     *  @param model_name: name of the model to copy
     *  @param version: version of the model
     * */
    void PullModel(std::string const &model_name, int version);

    /** @brief Pushed a model with the specified name
     *  @param model_name: name of the model to backup
     * */
    void PushModel(std::string const& model_name);

    /** @brief Collects the models from backup with the latest version
     *  @param path: path to the info file
     *  @return list of models
     * */
    std::list<fileType> GetSavedModels(std::string const& path);

    /** @brief Copies a model from backup to the project directory
     *  @param name: name of the model
     *  @param version: version of the model
     *  @param model_time: last modified time of the model
     * */
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
