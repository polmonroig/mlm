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
    else if(argument == "--help")Help();
    else if(argument == "--new"){
        if(argc < 4)EmptyEntry();
        bool default_c = true;
        if(argc == 5){
            if(argv[4] == "manual")default_c = false;
        }
        mlm::CreateProject(argv[2], argv[3], default_c);
    }
    else if(argument == "--add"){
        if(argc != 3)EmptyEntry();
        mlm::AddFile(argv[2]);
    }
    else{
        EmptyEntry();
    }


    return 0;
}
