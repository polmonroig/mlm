#ifndef UTILS_H
#define UTILS_H

// This file includes useful functions for the main app

// load std libraries
#include <iostream>
#include <fstream>
#include <string>

#define STD_EXIT_CODE 1

void Version(){
    std::cout << "ML Project Manager 1.0" << std::endl;
    exit(STD_EXIT_CODE);
}


void Help(std::string const& exec_dir){
    std::string dir = "";
    int i = exec_dir.size() - 1;
    while(exec_dir[i--] != '/');
    ++i;
    for(int j = 0; j <= i; ++j){
        dir += exec_dir[j];
    }
    std::ifstream help_file(dir + "help");
    std::string line;
    while(std::getline(help_file, line)){
        std::cout << line << std::endl;
    }
    help_file.close();
    exit(STD_EXIT_CODE);
}

void EmptyEntry(){
    std::cout << "No such command. Please use ml_manager --help" << std::endl;
    exit(STD_EXIT_CODE);
}

void InvalidEntry(std::string entry){
    std::cout << "No such command:" + entry + "Please use ml_manager --help" << std::endl;
    exit(STD_EXIT_CODE);
}


#endif
