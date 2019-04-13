/** @file utils.h
 *   *  @author Pol Monroig
 *  @brief This is the utils header, iit just defines some
 *         small and auxiliar functions
 * */

#ifndef UTILS_H
#define UTILS_H

// This file includes useful functions for the main app

// load std libraries
#include <iostream>
#include <fstream>
#include <string>


///@brief Version
void Version(){
    std::cout << "ML Project Manager 0.0.9.5" << std::endl;
    exit(EXIT_SUCCESS);
}


void Help(std::string const& exec_dir){
    std::string dir = "";
    auto i = exec_dir.size() - 1;
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
    exit(EXIT_SUCCESS);
}

void EmptyEntry(){
    std::cout << "No such command. Please use ml_manager --help" << std::endl;
    exit(EXIT_FAILURE);
}

void InvalidEntry(std::string entry){
    std::cout << "No such command:" + entry + "Please use ml_manager --help" << std::endl;
    exit(EXIT_FAILURE);
}


#endif
