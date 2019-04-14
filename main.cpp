/** @file main.cpp
 *  @author Pol Monroig
 * */

// load libraries
#include "utils.h"
#include "Manager/Manager.h"

int main(int argc, char* argv[]){
    // if wrong argument print InvalidEntry and exit
    if(argc < 2){
        EmptyEntry();
    }
    std::string argument = argv[1];
    if(argument == "--version")Version();
    else if(argument == "--help")Help(argv[0]);
    else if(argument == "new"){
        if(argc < 4)EmptyEntry();
        bool default_c = true;
        if(argc == 5){
            std::string cmp_string = argv[4];
            if(cmp_string == "--manual"){
                default_c = false;
            }
        }
        mlm::CreateProject(argv[2], argv[3], default_c);
    }
    else if(argument == "push"){
        if(argc != 3)EmptyEntry();
        mlm::PushFiles(argv[2]);
    }
    else if(argument == "display"){
        if(argc != 3)EmptyEntry();
        mlm::DisplayVersioning(argv[2]);
    }
    else if(argument == "pull"){
        if(argc < 3)EmptyEntry();
        std::string cmp_string = argv[2];
        if(cmp_string == "model"){
            if(argc < 4)EmptyEntry();
            if(argc == 5){
                std::string s = argv[4];
                // check if input is integer
                for(char const& c : s){
                    if(!isdigit(c))EmptyEntry();
                }
                mlm::PullModel(argv[3], std::stoi(s));
            }
            else {
                mlm::PullModel(argv[3], -1);
            }

        }
        else{
            std::string s = argv[2];
            if(argc == 4){
                std::string cmp_arg = argv[3];
                if(cmp_string == "--force") {
                    mlm::PullFiles(s, true);
                }
            }
            mlm::PullFiles(s, false);
        }
    }
    else{
        EmptyEntry();
    }


    return 0;
}
