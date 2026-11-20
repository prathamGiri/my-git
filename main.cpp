#include <string>
#include <iostream>
#include <sstream>

std::string command_parser(std::string &inp_command){
    std::string out = "Command taken";
    return out;
}

int main(){
    std::string inp_command, line;
    while (getline(std::cin, line))
    {
        inp_command+=line;
    }
    std::string result = command_parser(inp_command);
    std::cout << result;
}