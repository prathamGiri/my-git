#include <string>
#include <iostream>
#include <sstream>
#include "SHA1.hpp"

std::string command_parser(std::string &inp_command){
    std::string out = "Command taken";
    return out;
}

int main(){
    SHA1 sha;

    std::string input;
    std::getline(std::cin, input);
    std::cout << sha.block_stream(input) << std::endl;
    // std::string inp_command, line;
    // while (getline(std::cin, line))
    // {
    //     inp_command+=line;
    // }
    // std::string result = command_parser(inp_command);
    // std::cout << result;
}