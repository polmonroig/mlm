#include "Manager.h"


void mlm::ErrorAndExit(std::string const& error){
    std::cerr << error << std::endl;
    exit(1);
}

void mlm::WriteHeader(std::ofstream & file){
    file << "# File generated using ML Project Manager\n";
    file << "# DO NOT MODIFY THIS FILE\n";
}


void mlm::CreateConfigFile(std::string const& path, std::string const& content){
    std::ofstream file;
    file.open(path);
    if(!file)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(file);
    if(content != "") file << content;
    file.close();
}

void mlm::GenerateConfig(){
    // pre: we don't need to check if a folder exists because
    // we are working inside a new dir
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    mlm::CreateDir(conf_dir);
    // check if open fails!
    mlm::CreateConfigFile(mlm::JoinPaths(conf_dir, "info"));

    // Create .gitignore
    if(mlm::config[0] == '1'){
        std::ofstream gitignore_file;
        gitignore_file.open(mlm::JoinPaths(conf_dir, ".gitignore"), std::fstream::out);
        gitignore_file << "# backup and versioning";
        gitignore_file << "versioning/";
        gitignore_file.close();
    }
    // crete config and versioning folders
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
    mlm::CreateConfigFile(mlm::JoinPaths(models_dir, "info"));
    mlm::CreateConfigFile(mlm::JoinPaths(train_dir, "info"));
    mlm::CreateConfigFile(mlm::JoinPaths(test_dir, "info"));


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
    char answer;
    std::cout << "Do you want to create a git repository?[y/n] ";
    std::cin >> answer;
    if(answer == 'y')mlm::config[0] = '1';
    std::cout << "Do you want to create a data folder?[y/n] ";
    std::cin >> answer;
    if(answer == 'y')mlm::config[1] = '1';
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
    std::string abs_dir  = fs::absolute(fs::path(dir)).string();
    abs_dir.pop_back();
    mlm::project_path = mlm::JoinPaths(abs_dir, project_name);

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

std::string mlm::TimetoString(std::time_t t){
    char buff[20];
    strftime(buff, 20, "%Y%m%d%H%M%S", localtime(&t));
    return buff;
}

void mlm::UpdateFiles(std::string const& file_type){
    mlm::LoadGlobals();
    if(file_type == mlm::MODELS){
        // first we need to load the models in the current project
        auto models_dir = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
        std::set<fileType> files;
        for(auto& entry : boost::make_iterator_range(fs::directory_iterator(models_dir), {})){
            auto time = boost::filesystem::last_write_time(entry);
            files.insert(files.end(), fileType(entry.path().filename().string(), mlm::TimetoString(time), 1));
        }
        for(auto& s : files){
            std::cout << s.name << std::endl;
            std::cout << s.time << std::endl;
        }

        // next we need to load the saved models and compare them with the new ones
        auto versioning_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
        versioning_dir = mlm::JoinPaths(versioning_dir, "versioning/");
        versioning_dir = mlm::JoinPaths(versioning_dir, MODELS_DIR);
        std::ifstream in_file;
        in_file.open(mlm::JoinPaths(versioning_dir, "info"));
        std::string name, time, ver;
        std::list<fileType> saved_files;
        while(std::getline(in_file, name)){
            if(name[0] != '#'){
                std::getline(in_file, time);
                std::getline(in_file, ver);
                saved_files.insert(saved_files.end(), fileType(name, time, std::stoi(ver)));
            }
        }
        in_file.close();

        auto it = saved_files.begin();
        for(auto s : files){
            bool found = false;
            // loop until correct pos is found
            while(it != saved_files.end() && !found){
                if(s > *it)++it;
                else if(s == *it)found = true;
                else found = false;
            }
            // case inserting a new version

            if(!found && (--it) != saved_files.end() && it->name == s.name){
                s.version = it->version + 1;
                saved_files.insert(++it, s);
            }
            else if(!found){
                saved_files.insert(++it, s);
            }

            ++it;
        }
        std::ofstream out_file;
        out_file.open(mlm::JoinPaths(versioning_dir, "info"));
        mlm::WriteHeader(out_file);
        it = saved_files.begin();
        while(it != saved_files.end()){
            out_file << it->name << '\n' << it->time << '\n' << it->version << '\n';
            ++it;
        }
        out_file.close();

    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){

    }
    else{
        ErrorAndExit("Unknown file type");
    }


}
