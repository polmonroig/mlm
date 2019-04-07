#include "Manager.h"


void mlm::ErrorAndExit(std::string const& error){
    std::cerr << error << std::endl;
    exit(1);
}

void mlm::WriteHeader(std::ofstream & file){
    file << "# File generated using ML Project Manager\n";
    file << "# DO NOT MODIFY THIS FILE\n";
}

void mlm::GenerateConfig(){
    // pre: we don't need to check if a folder exists because
    // we are working inside a new dir
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    mlm::CreateDir(conf_dir);
    std::ofstream info_file;
    // check if open fails!
    info_file.open(mlm::JoinPaths(conf_dir, "info"), std::fstream::out);
    if(!info_file)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(info_file);
    info_file.close();

    // Create .gitignore
    if(mlm::config[0] == '1'){
        std::ofstream gitignore_file;
        gitignore_file.open(mlm::JoinPaths(conf_dir, ".gitignore"), std::fstream::out);
        gitignore_file << "# backup and versioning";
        gitignore_file << "versioning/";
        gitignore_file.close();
    }

    auto versioning_dir = mlm::JoinPaths(conf_dir, "versioning");
    auto models_dir = mlm::JoinPaths(versioning_dir, MODELS_DIR);
    auto test_dir = mlm::JoinPaths(versioning_dir, TEST_DIR);
    auto train_dir = mlm::JoinPaths(versioning_dir, TRAIN_DIR);
    mlm::CreateDir(versioning_dir);
    mlm::CreateDir(mlm::JoinPaths(versioning_dir, DATA_DIR));
    mlm::CreateDir(train_dir);
    mlm::CreateDir(test_dir);
    mlm::CreateDir(models_dir);

    // generate other config files
    std::ofstream other_conf;
    other_conf.open(mlm::JoinPaths(models_dir, "info"));
    if(!other_conf)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(other_conf);
    other_conf << "# models config file";
    other_conf << "0";
    other_conf.close();
    other_conf.open(mlm::JoinPaths(train_dir, "info"));
    if(!other_conf)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(other_conf);
    other_conf << "# train data config file";
    other_conf << "0";
    other_conf.close();
    other_conf.open(mlm::JoinPaths(test_dir, "info"));
    if(!other_conf)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(other_conf);
    other_conf << "# test data config file";
    other_conf << "0";
    other_conf.close();

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
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, DATA_DIR));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, TRAIN_DIR));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, TEST_DIR));
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, MODELS_DIR));

    mlm::SaveGlobals();

}

void mlm::LoadGlobals(){
    auto cwd = fs::current_path();
    auto conf_dir = mlm::JoinPaths(cwd.string(), CONFIG_DIR);
    if(!fs::exists(conf_dir)){
        mlm::ErrorAndExit("Coudn't find a project inside the current directory");
    }
    std::vector<std::string> variables(3);
    int i = 0;
    std::ifstream info_file(mlm::JoinPaths(conf_dir, "info"));
    while(i < 3){
        std::string line;
        std::getline(info_file, line);
        if(line[0] != '#'){
            variables[i++] = line;
        }
    }
    info_file.close();
    mlm::config = variables[0];
    mlm::project_name = variables[1];
    mlm::project_path = variables[2];

}

void mlm::SaveGlobals(){
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    std::ofstream info_file;
    info_file.open(mlm::JoinPaths(conf_dir, "info"), std::fstream::app);
    info_file << mlm::config << '\n';
    info_file << mlm::project_name << '\n';
    info_file << mlm::project_path << '\n';

    info_file.close();
}

void mlm::DisplayVersioning(std::string const& file_type){
    auto cwd = fs::current_path();
    auto conf_dir = mlm::JoinPaths(cwd.string(), CONFIG_DIR);
    if(!fs::exists(conf_dir)){
        mlm::ErrorAndExit("Coudn't find a project inside the current directory");
    }
    mlm::LoadGlobals();
    if(file_type == mlm::MODELS){

    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){

    }
    else{
        ErrorAndExit("Unknown file type");
    }
}

void mlm::UpdateFiles(std::string const& file_type){
    auto cwd = fs::current_path();
    auto conf_dir = mlm::JoinPaths(cwd.string(), CONFIG_DIR);
    if(!fs::exists(conf_dir)){
        mlm::ErrorAndExit("Coudn't find a project inside the current directory");
    }
    mlm::LoadGlobals();
    if(file_type == mlm::MODELS){

    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){

    }
    else{
        ErrorAndExit("Unknown file type");
    }

}
