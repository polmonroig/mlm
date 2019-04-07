#include "Manager.h"


void mlm::ErrorAndExit(std::string const& error){
    std::cerr << error << std::endl;
    exit(1);
}

void mlm::GenerateConfig(){
    // pre: we don't need to check if a folder exists because
    // we are working inside a new dir
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    mlm::CreateDir(conf_dir);
    std::ofstream info_file, conf_file;
    // check if open fails!

    info_file.open(mlm::JoinPaths(conf_dir, "info"), std::fstream::out);
    if(!info_file)ErrorAndExit("Error while creating config");
    info_file << "# File generated using ML Project Manager\n";
    info_file << "# DO NOT MODIFY THIS FILE\n";
    // if default config is set, get info drom default_config file
    info_file.close();

}

std::string mlm::JoinPaths(std::string const& p1, std::string const& p2){
    auto path = p1;
    if(p1[p1.size() - 1] == '/')path += p2;
    else path += "/" + p2;
    return path;
}

void mlm::CreateDir(std::string const& dir){
    fs::path p_dir(dir);
    if (!fs::create_directory(p_dir)){
        mlm::ErrorAndExit("Error while creating project directory");
    }
}

void mlm::Configure(){
    mlm::config = "";
}

void mlm::CreateProject(std::string const& name, std::string const& dir, bool default_config){

    // check if project is viable
    if(!fs::exists(dir)){
        mlm::ErrorAndExit("The specified directory does not exist");
    }
    if(fs::exists(mlm::project_path)){
        mlm::ErrorAndExit("Cannot create project, specified directory already exists");
    }



    // set global variables
    mlm::project_name = name;
    mlm::default_config = default_config;
    mlm::project_path = mlm::JoinPaths(dir, project_name);

    if(mlm::default_config){
        std::ifstream conf_file;
        conf_file.open("default_config");
        if(!conf_file)ErrorAndExit("Error while creating config");
        // set default config
        std::getline(conf_file, config);
        conf_file.close();
    }
    else{
        mlm::Configure();
    }


    // create project main folder
    mlm::CreateDir(mlm::project_path);

    // generate config files
    mlm::GenerateConfig();

    // add extra directories
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, "data"));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, "data/train"));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, "data/test"));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, "models"));

    mlm::SaveGlobals();

}

void mlm::LoadGlobals(){

}
inline
void mlm::SaveGlobals(){
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    std::ofstream info_file;
    info_file.open(mlm::JoinPaths(conf_dir, "info"), std::fstream::out);
    info_file << "Config: " << mlm::config << '\n';
    info_file << "Project Name: " << mlm::project_name << '\n';
    info_file << "Project Path: " << mlm::project_path << '\n';

    info_file.close();
}

void mlm::AddFile(std::string const& file_path){

    if(!fs::exists(file_path)){
        mlm::ErrorAndExit("The specified file does not exist");
    }

}
