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
            if(argv[4] == "manual")default_c = false;
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
    else if(argument == "verify"){
        mlm::Verify();
    }
    else if(argument == "pull"){
        //if(argc != 3)EmptyEntry();
        mlm::PullFiles("models");
    }
    else{
        EmptyEntry();
    }


    return 0;
}
