#include "mygit/sha1.hpp"

#include <string>
#include <iostream>
#include <sstream>


std::string command_parser(std::string &inp_command){
    std::string out = "Command taken";
    return out;
}

int main(){
    std::string input;
    std::getline(std::cin, input);
    std::cout << sha1(input) << std::endl;
    // std::string inp_command, line;
    // while (getline(std::cin, line))
    // {
    //     inp_command+=line;
    // }
    // std::string result = command_parser(inp_command);
    // std::cout << result;
}