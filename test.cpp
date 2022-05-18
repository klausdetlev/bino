#include <iostream>
#include <bino.hpp>

int main(int argc, char** argv){
    bino::write("test.dat",15);
    std::cout << bino::read<int>("test.dat") << std::endl;

    std::string str{"Teststring"};
    bino::write("test.dat",str);
    std::cout << "\"" << bino::read<std::string>("test.dat") << "\"" << std::endl;

    std::vector<std::unordered_map<int,int>> v{std::unordered_map<int,int>{{4,3},{2,1},{3,1}},std::unordered_map<int,int>{{15,16},{17,18}}};
    bino::write("test.dat",v);
    std::vector<std::unordered_map<int,int>> v2 = bino::read<std::vector<std::unordered_map<int,int>>>("test.dat");
    for(const std::unordered_map<int,int>& m: v2){
        for(const auto& [k,v]: m){
            std::cout << "\"" << k << "\":\"" << v << "\"" << std::endl;
        }
        std::cout << std::endl;
    }

    std::tuple<int,int,int> tup(1,2,3);
    bino::write("test.dat",tup);
    std::tuple<int,int,int> test = bino::read<std::tuple<int,int,int>>("test.dat");
    std::cout << std::get<0>(test) << " " << std::get<1>(test) << " " << std::get<2>(test) << std::endl;
    return 0;
}