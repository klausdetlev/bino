#include <iostream>
#include <bino.hpp>

int main(int argc, char** argv){
    bino::write("test.dat",15);
    std::cout << bino::read<int>("test.dat") << std::endl;

    std::string str{"Teststring"};
    bino::write("test.dat",str);
    std::cout << "\"" << bino::read<std::string>("test.dat") << "\"" << std::endl;
    return 0;
}