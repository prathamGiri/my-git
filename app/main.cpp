#include "mygit/commands.hpp"

#include <string>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){
    if (argc < 2)
    {
        std::cerr << "Usage: mygit <command> [args]\n";
        return 1;
    }
    std::string command = argv[1];
    std::vector<std::string> args(argv+2, argv+argc); 
    if (command == "hash-object") return hash_object(args);

    std::cerr << "Command Not Found\n";
    return 1;
}