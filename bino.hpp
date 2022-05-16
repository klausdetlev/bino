#ifndef __BINO_HPP
#define __BINO_HPP 1

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

template<typename t> struct bio;

namespace bino{
    template<typename t> concept pod = std::is_trivially_copyable<t>::value;
    template<typename t> concept nonpod = !std::is_trivially_copyable<t>::value;
    template<typename t> concept writer = std::is_same<std::fstream,t>::value || std::is_same<std::ofstream,t>::value;
    template<typename t> concept reader = std::is_same<std::fstream,t>::value || std::is_same<std::ifstream,t>::value;

    template<writer stream, typename t> stream& write(stream& out, const t& elem){
        bio<t>::write(out,elem);
        return out;
    }
    template<typename t> void write(const std::string& dst, const t& elem, const std::ios_base::openmode om = {}){
        std::ofstream out(dst.c_str(), om | std::ios::out | std::ios::binary);
        bio<t>::write(out,elem);
    }
    template<reader stream, typename t> stream& read(stream& in, t& elem){
        bio<t>::read(in,elem);
        return in;
    }
    template<typename t> void read(const std::string& src, t& elem, const std::ios_base::openmode om = {}){
        std::ifstream in(src.c_str(), om | std::ios::binary);
        bio<t>::read(in,elem);
    }
    template<reader stream, typename t> t read(stream& in){
        return bio<t>::read(in);
    }
    template<typename t> t read(const std::string& src, const std::ios_base::openmode om = {}){
        std::ifstream in(src.c_str(), om | std::ios::binary);
        return bio<t>::read(in);
    }
}

template<bino::pod t> struct bio<t>{
    template<bino::writer stream> inline static stream& write(stream& out, const t& elem){
        out.write(reinterpret_cast<const char*>(&elem),sizeof(t));
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, t& elem){
        in.read(reinterpret_cast<char*>(&elem),sizeof(t));
        return in;
    }
    template<bino::reader stream> inline static t read(stream& in){
        t retval;
        in.read(reinterpret_cast<char*>(&retval),sizeof(t));
        return retval;
    }
};

template<bino::pod t> struct bio<std::basic_string<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::basic_string<t>& str){
        bio<typename std::basic_string<t>::size_type>::write(out,str.length());
        out.write(reinterpret_cast<const char*>(str.c_str()),sizeof(t)*str.length());
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::basic_string<t>& str){
        str.resize(bio<typename std::basic_string<t>::size_type>::read(in));
        in.read(reinterpret_cast<char*>(&str[0]),sizeof(t)*str.length());
    }
    template<bino::reader stream> inline static std::basic_string<t> read(stream& in){
        std::basic_string<t> retval;
        retval.resize(bio<typename std::basic_string<t>::size_type>::read(in));
        in.read(reinterpret_cast<char*>(&retval[0]),sizeof(t)*retval.length());
        return retval;
    }
};

template<bino::nonpod t> struct bio<std::basic_string<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::basic_string<t>& str){
        bio<typename std::basic_string<t>::size_type>::write(out,str.length());
        for(typename std::basic_string<t>::const_iterator i = str.begin();i!=str.end();++i) bio<t>::write(out,*i);
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::basic_string<t>& str){
        str.resize(bino<typename std::basic_string<t>::size_type>::read(in));
        for(typename std::basic_string<t>::iterator i = str.begin();i!=str.end();++i) bio<t>::read(in,*i);
        return in;
    }
    template<bino::reader stream> inline static std::basic_string<t> read(stream& in){
        std::basic_string<t> str;
        str.resize(bino<typename std::basic_string<t>::size_type>::read(in));
        for(typename std::basic_string<t>::iterator i = str.begin();i!=str.end();++i) bio<t>::read(in,*i);
        return str;
    }
};

#endif