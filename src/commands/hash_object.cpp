#include "mygit/sha1.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int hash_object(std::vector<std::string> &args){
    size_t args_c = args.size();
    if (args_c != 1) 
    {
        std::cerr << "Usage: mygit hash-object [option] [filename / text]\n";
        return 1;
    }

    if (args[0] == "--stdin")
    {
        std::string content;
        std::cin >> content;
        std::cout << sha1("blob " + std::to_string(content.size()) + "\0" + content) << "\n";
        return 0;
    }else if (args[0][0]  == '-')
    {
        std::cerr << "Command Not Found\n";
        return 1;
    }else {
        
        std::ifstream in(args[0], std::ios::binary);
        std::string file_content(
            // this is an iterator that reads characters directly 
            // from a stream's underlying buffer
            (std::istreambuf_iterator<char>(in)), 
            std::istreambuf_iterator<char>()
        );
        file_content = "blob " + std::to_string(file_content.size()) + "\0" + file_content;
        std::cout << sha1(file_content) << "\n";
        return 0;
    }
}