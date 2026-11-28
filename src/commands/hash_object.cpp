#include "mygit/sha1.hpp"
#include "mygit/zlib.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;
namespace {
    int add_header(std::string &filename, std::string &file_content){
        std::ifstream in(filename, std::ios::binary);
        if (!in) return 1;
        file_content(
            // this is an iterator that reads characters directly 
            // from a stream's underlying buffer
            (std::istreambuf_iterator<char>(in)), 
            std::istreambuf_iterator<char>()
        );
        file_content = "blob " + std::to_string(file_content.size()) + '\0' + file_content;
        return 0;
    }
}

int hash_object(std::vector<std::string> &args, std::istream &input = std::cin){
    size_t args_c = args.size();
    if (args_c < 1) 
    {
        std::cerr << "Usage: mygit hash-object [option] [filename / text]\n";
        return 1;
    }

    if (args[0]  == '-w')
    {
        if (args_c < 2)
        {
            std::cerr << "Usage: mygit hash-object -w [filename]\n";
            return 1;
        }
        if (!fs::exists(".git/objects"))
        {
            std::cerr << ".git Folder not found: Use git init\n";
            return 1; 
        }
        std::string file_content;
        if(add_header(args[1], file_content)){
            std::cerr << "File not found\n";
            return 1;
        }
        std::string compressed_data = zcompress(file_content);
        std::string content_hash = sha1(file_content);
        fs::create_directory(".git/objects/"+content_hash.substr(0,2));
        std::ofstream out(
            ".git/objects/"
            +content_hash.substr(0,2)
            +"/"
            +content_hash.substr(2,content_hash.size()-2),
            std::ios::binary
        );
        out >> compressed_data;
        std::cout << content_hash << "\n";
        return 0;
    }else if (args[0] == "--stdin")
    {
        std::string content(
            (std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()
        );
        std::cout << sha1("blob " + std::to_string(content.size()) + '\0' + content) << "\n";
        return 0;
    }else if (args[0][0]  == '-')
    {
        std::cerr << "Command Not Found\n";
        return 1;
    }else {
        
        std::string file_content;
        if(add_header(args[0], file_content)){
            std::cerr << "File not found\n";
            return 1;
        }
        std::cout << sha1(file_content) << "\n";
        return 0;
    }
}