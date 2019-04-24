/** @file Manager.cpp
 *  @author Pol Monroig
 * */

#include "Manager.h"


void mlm::ErrorAndExit(std::string const& error){
    std::cerr << error << std::endl;
    exit(EXIT_FAILURE);
}

void mlm::WriteHeader(fs::ofstream & file){
    file << "# File generated using ML Project Manager\n";
    file << "# DO NOT MODIFY THIS FILE\n";
}


void mlm::CreateConfigFile(std::string const& path, std::string const& content){
    fs::ofstream file;
    file.open(path);
    if(!file)ErrorAndExit("Error while creating config");
    mlm::WriteHeader(file);
    if(!content.empty()) file << content;
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
        fs::ofstream gitignore_file;
        gitignore_file.open(mlm::JoinPaths(mlm::project_path, ".gitignore"), fs::ofstream::app);
        gitignore_file << "# mlm\n";
        gitignore_file << CONFIG_DIR"/";
        gitignore_file.close();
    }
    // crete config and backup folders
    auto models_dir = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    mlm::CreateDir(mlm::backup_path);

    if(mlm::config[1] == '1'){
        auto test_dir = mlm::JoinPaths(mlm::backup_path, TEST_DIR);
        auto train_dir = mlm::JoinPaths(mlm::backup_path, TRAIN_DIR);
        auto data_dir = mlm::JoinPaths(mlm::backup_path, DATA_DIR);
        mlm::CreateDir(data_dir);
        mlm::CreateDir(train_dir);
        mlm::CreateDir(test_dir);
        mlm::CreateConfigFile(mlm::JoinPaths(train_dir, "info"));
        mlm::CreateConfigFile(mlm::JoinPaths(test_dir, "info"));
    }

    mlm::CreateDir(models_dir);

    // generate other config files
    mlm::CreateConfigFile(mlm::JoinPaths(models_dir, "info"));



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
    std::cout << "Manual configure enabled" << std::endl;
    char answer;
    std::cout << "Do you want to create a git repository?[y/n] ";
    std::cin >> answer;
    if(answer == 'y')mlm::config[0] = '1';
    std::cout << "Do you want to create a data folder?[y/n] ";
    std::cin >> answer;
    if(answer == 'y')mlm::config[1] = '1';
    std::string path;
    while(path.empty() or !fs::exists(path)){
        std::cout << "Where do you want the backup directory? (write default for default)";
        std::cin >> path;
        if(path == "default"){
            path = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
            break;
        }
    }
    mlm::backup_path = mlm::JoinPaths(path, "backup/");


}

void mlm::CreateProject(std::string const& name, std::string const& dir, bool default_config){

    // set global variables
    mlm::project_name = name;
    mlm::default_config = default_config;
    std::string abs_dir = fs::absolute(fs::path(dir)).string();
    abs_dir.pop_back();
    mlm::project_path = mlm::JoinPaths(abs_dir, project_name);

    // check if project is viable
    if(!fs::exists(dir)){
        mlm::ErrorAndExit("The specified directory does not exist");
    }
    if(fs::exists(mlm::project_path)){
        std::cout << "The specified directory already exists." << std::endl
                  << "Continue?[y/n]" << std::endl;
        char c;
        std::cin >> c;
        if(c != 'y')exit(EXIT_FAILURE);
    }
    else{
        // create project main folder
        mlm::CreateDir(mlm::project_path);
    }


    if(mlm::default_config){
        fs::ifstream conf_file;
        conf_file.open("default_config");
        if(conf_file.fail())ErrorAndExit("Error while creating file");
        // set default config
        mlm::backup_path = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
        mlm::backup_path = mlm::JoinPaths(mlm::backup_path, "backup/");
        std::getline(conf_file, config);
        conf_file.close();
    }
    else{
        mlm::Configure();
    }




    // generate config files
    mlm::GenerateConfig();

    // add extra directories
    if(mlm::config[1] == '1'){
        mlm::CreateDir(mlm::JoinPaths(mlm::project_path, DATA_DIR));
        mlm::CreateDir(mlm::JoinPaths(mlm::project_path, TRAIN_DIR));
        mlm::CreateDir(mlm::JoinPaths(mlm::project_path, TEST_DIR));
    }
    mlm::CreateDir(mlm::JoinPaths(mlm::project_path, MODELS_DIR));
    mlm::SaveGlobals();

}

void mlm::LoadGlobals(){

    //auto cwd = fs::current_path();
    //auto conf_dir = mlm::JoinPaths(cwd.string(), CONFIG_DIR);
    auto conf_dir = mlm::JoinPaths("/home/polmonroig/Documents/mlm/project/", CONFIG_DIR);
    if(!fs::exists(conf_dir)){
        mlm::ErrorAndExit("Coudn't find a project inside the current directory");
    }
    auto n_variables = 4;
    std::vector<std::string> variables(n_variables);
    int i = 0;
    fs::ifstream info_file(mlm::JoinPaths(conf_dir, "info"));
    while(i < n_variables){
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
    mlm::backup_path = variables[3];

}

void mlm::SaveGlobals(){
    auto conf_dir = mlm::JoinPaths(mlm::project_path, CONFIG_DIR);
    fs::ofstream info_file;
    info_file.open(mlm::JoinPaths(conf_dir, "info"));
    if(info_file.fail())ErrorAndExit("Error while opening file");
    mlm::WriteHeader(info_file);
    info_file << mlm::config << '\n';
    info_file << mlm::project_name << '\n';
    info_file << mlm::project_path << '\n';
    info_file << mlm::backup_path << '\n';

    info_file.close();
}

void mlm::DisplayVersioning(std::string const& file_type){
    mlm::LoadGlobals();
    if(file_type == mlm::MODELS){
        auto backup_models = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
        fs::ifstream in_file;
        in_file.open(mlm::JoinPaths(backup_models, "info"));
        if(in_file.fail())ErrorAndExit("Error while opening file");
        std::string name, time, ver;
        while(std::getline(in_file, name)){
            if(name[0] != '#'){
                std::getline(in_file, time);
                std::getline(in_file, ver);
                std::cout << name << " - " << time << " - " << ver << std::endl;
            }
        }
        in_file.close();
    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){

    }
    else{
        ErrorAndExit("Unknown file type");
    }
}

std::string mlm::TimeToString(std::time_t t){
    char buff[20];
    strftime(buff, 20, "%Y%m%d %H%M%S", localtime(&t));
    return buff;
}

void mlm::SetModelVersion(int v){
    auto pos = CONFIG_LEN - 2;
    std::string s_version = std::to_string(mlm::config[pos] - '0') + std::to_string(mlm::config[pos + 1] - '0');
    int i_version =  std::stoi(s_version);
    if(i_version == v){
        i_version++;
        s_version = std::to_string(i_version);
        if(i_version >= 10){
            mlm::config[pos] = s_version[0];
            mlm::config[pos + 1] = s_version[1];
        }
        else {
            mlm::config[pos] = '0';
            mlm::config[pos + 1] = s_version[0];
        }

    }
}


std::set<mlm::fileType> mlm::GetFiles(std::string const& path){
    std::set<mlm::fileType> files;
    for(auto& entry : boost::make_iterator_range(fs::directory_iterator(path), {})){
        auto time = fs::last_write_time(entry);
        files.insert(files.end(), fileType(entry.path().filename().string(), mlm::TimeToString(time), 1));
    }
    return files;
}


void mlm::PullModel(std::string const &model_name, int version){
    mlm::LoadGlobals();
    fs::ifstream file;
    auto backup_models = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    file.open(mlm::JoinPaths(backup_models, "info"));
    if(file.fail())ErrorAndExit("Error while opening file");
    std::string tmp, ver, model_time;
    while(std::getline(file, tmp)){
        if(tmp[0] != '#'){
            if(tmp < model_name){
                std::getline(file, model_time);
                std::getline(file, ver);
            }
            else if(tmp == model_name){
                std::getline(file, model_time);
                std::getline(file, ver);
                if(version == stoi(ver)){
                    break;
                }
            }
            else if(tmp > model_name){
                break;
            }

        }
    }
    file.close();
    if(version > stoi(ver))ErrorAndExit("There isn't any model with the specified name and version");
    auto current_model = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
    current_model = mlm::JoinPaths(current_model, model_name);
    if(fs::exists(current_model)){
        if(mlm::TimeToString(fs::last_write_time(current_model)) > model_time){
            ErrorAndExit("You currently have a newer version of the requested model, push to update");
        }
    }
    backup_models = mlm::JoinPaths(backup_models, ver);
    backup_models = mlm::JoinPaths(backup_models, model_name);
    if(!fs::exists(backup_models))ErrorAndExit("There isn't any model with the specified name and version");
    mlm::CopyModel(model_name, stoi(ver), model_time);

}

std::list<mlm::fileType> mlm::GetInfoFiles(std::string const& path){
    fs::ifstream input_file;
    input_file.open(path);
    if(input_file.fail())ErrorAndExit("Error while opening file");
    std::string name, time, ver;
    std::list<mlm::fileType> files;
    while(std::getline(input_file, name)){
        if(name[0] != '#'){
            std::getline(input_file, time);
            std::getline(input_file, ver);
            files.insert(files.end(), fileType(name, time, std::stoi(ver)));
        }
    }
    input_file.close();
    return files;
}

void mlm::UpdateInfoFiles(std::set<mlm::fileType> const& files, std::list<mlm::fileType>& info_files){
    auto models_dir = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
    bool uptodate = true; // keeps track if there is a new model
    auto it = info_files.begin();
    for(auto s : files){
        bool found = false;
        // loop until correct pos is found
        while(it != info_files.end() && !found){
            if(s > *it)++it;
            else if(s == *it)found = true;
            else break;
        }
        // case inserting a new version
        --it;
        if(!found && it != info_files.end() && it->name == s.name){
            mlm::SetModelVersion(it->version);
            s.version = it->version + 1;
            mlm::AddModel(mlm::JoinPaths(models_dir, s.name), s.name, s.version);
            info_files.insert(++it, s);
            std::cout << s.name << " updated" << std::endl;
            uptodate = false;
        }
        else if(!found){
            ++it;
            mlm::AddModel(mlm::JoinPaths(models_dir, s.name),s.name,  s.version);
            info_files.insert(it, s);
            std::cout << s.name << " added" << std::endl;
            uptodate = false;
        }
        else {
            ++it; // if found increment iterator
        }
    }
    if(uptodate)std::cout << "All your models are already up to date!" << std::endl;
}

void mlm::SaveFilesToInfo(std::list<mlm::fileType> const& files, std::string const& path){
    fs::ofstream out_file;
    out_file.open(path);
    mlm::WriteHeader(out_file);
    auto it = files.begin();
    while(it != files.end()){
        out_file << it->name << '\n' << it->time << '\n' << it->version << '\n';
        ++it;
    }
    out_file.close();
}

void mlm::PushModels(){
    // first we need to load the models in the current project
    auto models_dir = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
    auto files = mlm::GetFiles(models_dir);

    // next we need to load the saved models
    auto models_backup = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    auto saved_files = mlm::GetInfoFiles(mlm::JoinPaths(models_backup, "info"));

    // then add the models new models to the saved ones
    mlm::UpdateInfoFiles(files, saved_files);

    // save the models list to backup info file
    mlm::SaveFilesToInfo(saved_files, mlm::JoinPaths(models_backup, "info"));
}

void mlm::AddModel(std::string const& source_path, std::string const& model_name, int model_version){
    auto destination_path = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    destination_path = mlm::JoinPaths(destination_path, std::to_string(model_version));
    if(!fs::exists(destination_path)){
        mlm::CreateDir(destination_path);
    }
    fs::copy_file(source_path, mlm::JoinPaths(destination_path, model_name));
}

void mlm::PushFiles(std::string const& file_type){
    mlm::LoadGlobals();
    if(file_type == mlm::MODELS){
        mlm::PushModels();
    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){
        mlm::PushModels();
    }
    else{
        ErrorAndExit("Unknown file type");
    }
    // finally save global variables
    mlm::SaveGlobals();
}

std::list<mlm::fileType> mlm::GetSavedModels(std::string const& path){

    auto pos = CONFIG_LEN - 2;
    std::string s_version = std::to_string(mlm::config[pos] - '0') + std::to_string(mlm::config[pos + 1] - '0');
    fs::ifstream input_file, check_file;

    // create files
    input_file.open(mlm::JoinPaths(path, "info"));
    check_file.open(mlm::JoinPaths(path, "info"));
    if(input_file.fail() || check_file.fail()){
        mlm::ErrorAndExit("Error while opening file");
    }
    // create temporal fileType variables
    std::string name, time, ver;
    std::string tmp_version, tmp_name, tmp_time;
    // advance second iterator
    std::getline(check_file, tmp_name);
    while(tmp_name[0] == '#')std::getline(check_file, tmp_name);
    std::getline(check_file, tmp_time);
    std::getline(check_file, tmp_version);

    // iterate through file
    std::list<mlm::fileType> files;
    while(std::getline(input_file, name)){
        if(name[0] != '#'){
            std::getline(input_file, time);
            std::getline(input_file, ver);

            std::getline(check_file, tmp_name);
            std::getline(check_file, tmp_time);
            while(std::getline(check_file, tmp_version) && tmp_version != "1"){
                std::getline(input_file, name);
                std::getline(input_file, time);
                std::getline(input_file, ver);
                std::getline(check_file, tmp_name);
                std::getline(check_file, tmp_time);
                ver = tmp_version;
            }
            files.insert(files.end(), fileType(name, time, std::stoi(ver)));
        }
    }
    input_file.close();
    return files;
}

void mlm::PullModels(){
    // load backup models
    auto models_backup = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    auto saved_models = mlm::GetSavedModels(models_backup);

    // load models of the current project
    auto models_dir = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
    auto files = mlm::GetFiles(models_dir);
    auto it = files.begin();

    for(auto const& f : saved_models){
        if(it != files.end() && it->name < f.name){
            ++it;// file has not been tracked
        }
        else if(it != files.end() && it->name == f.name){
            if(it->time < f.time){ // currently using an old version
                if(force_overwrite){ // force flag is active
                    // copy file
                    mlm::CopyModel(f.name, f.version, it->name);
                }
            }
            else if(it->time > f.time){
                std::cout << "Warning: you are currently using a newer version of "
                << it->name << ", use push to update" << std::endl;
            }
            ++it;
        }
        else{
            mlm::CopyModel(f.name, f.version, f.time);
        }
    }
}

void mlm::CopyModel(std::string const& name, int version, std::string const& model_time){
    auto models_backup = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    auto models_dir = mlm::JoinPaths(mlm::project_path, MODELS_DIR);

    auto p = mlm::JoinPaths(models_backup, std::to_string(version));
    p = mlm::JoinPaths(p, name);
    fs::copy_file(p, mlm::JoinPaths(models_dir, name), fs::copy_option::overwrite_if_exists);
    struct std::tm tm = {0};
    std::istringstream ss(model_time);
    ss >> std::get_time(&tm, "%Y%m%d %H%M%S");
    tm.tm_isdst=-1;
    time_t time = std::mktime(&tm);
    fs::last_write_time(mlm::JoinPaths(models_dir, name), time);
}

void mlm::PullFiles(std::string const &file_type, bool force) {
    mlm::LoadGlobals();
    mlm::force_overwrite = force;
    if(file_type == mlm::MODELS){
        mlm::PullModels();
    }
    else if(file_type == mlm::DATA){

    }
    else if(file_type == mlm::ALL){
        mlm::PullModels();
    }
    else{
        ErrorAndExit("Unknown file type");
    }
    // finally save global variables
    mlm::SaveGlobals();
}



void mlm::PushModel(std::string const& model_name){
    mlm::LoadGlobals();
    auto model_path = mlm::JoinPaths(mlm::project_path, MODELS_DIR);
    model_path = mlm::JoinPaths(model_path, model_name);
    if(!fs::exists(model_path)){
        ErrorAndExit("The specified model doesn't exist in the models directory");
    }

    // next we need to load the saved models
    auto models_backup = mlm::JoinPaths(mlm::backup_path, MODELS_DIR);
    auto saved_files = mlm::GetInfoFiles(mlm::JoinPaths(models_backup, "info"));

    std::set<fileType> files;
    files.insert(fileType(model_name, mlm::TimeToString(fs::last_write_time(model_path)), 1));
    // then add the models new models to the saved ones
    mlm::UpdateInfoFiles(files, saved_files);

    // save the models list to backup info file
    mlm::SaveFilesToInfo(saved_files, mlm::JoinPaths(models_backup, "info"));

}