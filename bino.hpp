#ifndef __BINO_HPP
#define __BINO_HPP 1

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

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

template<bino::pod t> struct bio<std::vector<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::vector<t>& vec){
        bio<typename std::vector<t>::size_type>::write(out,vec.size());
        out.write(reinterpret_cast<const char*>(vec.data()),vec.size()*sizeof(t));
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::vector<t>& vec){
        vec.resize(bio<typename std::vector<t>::size_type>::read(in));
        in.read(reinterpret_cast<char*>(vec.data()),sizeof(t)*vec.size());
        return in;
    }
    template<bino::reader stream> inline static std::vector<t> read(stream& in){
        std::vector<t> vec(bio<typename std::vector<t>::size_type>::read(in));
        in.read(reinterpret_cast<char*>(vec.data()),sizeof(t)*vec.size());
        return vec;
    }
};

template<bino::nonpod t> struct bio<std::vector<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::vector<t>& vec){
        bio<typename std::vector<t>::size_type>::write(out,vec.size());
        for(typename std::vector<t>::const_iterator i=vec.begin();i!=vec.end();++i) bio<t>::write(out,*i);
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::vector<t>& vec){
        vec.resize(bio<typename std::vector<t>::size_type>::read(in));
        for(typename std::vector<t>::iterator i=vec.begin();i!=vec.end();++i) bio<t>::read(in,*i);
        return out;
    }
    template<bino::reader stream> inline static std::vector<t> read(stream& in){
        std::vector<t> vec(bio<typename std::vector<t>::size_type>::read(in));
        for(typename std::vector<t>::iterator i=vec.begin();i!=vec.end();++i) bio<t>::read(in,*i);
        return vec;
    }
};

template<bino::pod t, size_t n> struct bio<std::array<t,n>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::array<t,n>& a){
        out.write(reinterpret_cast<const char*>(a.data()),n*sizeof(t));
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::array<t,n>& a){
        in.read(reinterpret_cast<char*>(a.data()),n*sizeof(t));
        return in;
    }
    template<bino::reader stream> inline static std::array<t,n> read(stream& in){
        std::array<t,n> a;
        in.read(reinterpret_cast<char*>(a.data()),n*sizeof(t));
        return a;
    }
};

template<bino::nonpod t, size_t n> struct bio<std::array<t,n>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::array<t,n>& a){
        for(typename std::array<t,n>::const_iterator i=a.begin();i!=a.end();++i) bio<t>::write(out,*i);
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::array<t,n>& a){
        for(typename std::array<t,n>::iterator i=a.begin();i!=a.end();++i) bio<t>::read(in,*i);
        return in;
    }
    template<bino::reader stream> inline static stream& read(stream& in){
        std::array<t,n> a;
        for(typename std::array<t,n>::iterator i=a.begin();i!=a.end();++i) bio<t>::read(in,*i);
        return a;
    }
};

template<typename t> struct bio<std::unordered_set<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::unordered_set<t>& s){
        bio<typename std::unordered_set<t>::size_type>::write(out,s.size());
        for(typename std::unordered_set<t>::const_iterator i=s.begin();i!=s.end();++i) bio<t>::write(out,*i);
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::unordered_set<t>& s){
        typename std::unordered_set<t>::size_type size;
        s.clear();
        for(bio<typename std::unordered_set<t>::size_type>::read(in,size);size>0;--size) s.emplace(bio<t>::read(in));
        return in;
    }
    template<bino::reader stream> inline static std::unordered_set<t> read(stream& in){
        std::unordered_set<t> s;
        typename std::unordered_set<t>::size_type size;
        for(bio<typename std::unordered_set<t>::size_type>::read(in,size);size>0;--size) s.emplace(bio<t>::read(in));
        return s;
    }
};

template<typename t> struct bio<std::set<t>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::set<t>& s){
        bio<typename std::set<t>::size_type>::write(out,s.size());
        for(typename std::set<t>::const_iterator i=s.begin();i!=s.end();++i) bio<t>::write(out,*i);
        return out;
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::set<t>& s){
        typename std::set<t>::size_type size;
        s.clear();
        for(bio<typename std::set<t>::size_type>::read(in,size);size>0;--size) s.emplace(bio<t>::read(in));
        return in;
    }
    template<bino::reader stream> inline static std::set<t> read(stream& in){
        std::set<t> s;
        typename std::set<t>::size_type size;
        for(bio<typename std::set<t>::size_type>::read(in,size);size>0;--size) s.emplace(bio<t>::read(in));
        return s;
    }
};

template<typename k, typename v> struct bio<std::unordered_map<k,v>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::unordered_map<k,v>& m){
        bio<typename std::unordered_map<k,v>::size_type>::write(out,m.size());
        for(typename std::unordered_map<k,v>::const_iterator i=m.begin();i!=m.end();++i){
            bio<k>::write(out,i->first);
            bio<v>::write(out,i->second);
        }
        out.close();
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::unordered_map<k,v>& m){
        k tmp;
        v tmp2;
        typename std::unordered_map<k,v>::size_type size;
        m.clear();
        for(bio<typename std::unordered_map<k,v>::size_type>::read(in,size);size>0;--size){
            bio<k>::read(in,tmp);
            bio<v>::read(in,tmp2);
            m.emplace(std::move(tmp),std::move(tmp2));
        }
        return in;
    } 
    template<bino::reader stream> inline static std::unordered_map<k,v> read(stream& in){
        k tmp;
        v tmp2;
        typename std::unordered_map<k,v>::size_type size;
        std::unordered_map<k,v> m;
        for(bio<typename std::unordered_map<k,v>::size_type>::read(in,size);size>0;--size){
            bio<k>::read(in,tmp);
            bio<v>::read(in,tmp2);
            m.emplace(std::move(tmp),std::move(tmp2));
        }
        return m;
    }
};

template<typename k, typename v> struct bio<std::map<k,v>>{
    template<bino::writer stream> inline static stream& write(stream& out, const std::map<k,v>& m){
        bio<typename std::map<k,v>::size_type>::write(out,m.size());
        for(typename std::map<k,v>::const_iterator i=m.begin();i!=m.end();++i){
            bio<k>::write(out,i->first);
            bio<v>::write(out,i->second);
        }
        out.close();
    }
    template<bino::reader stream> inline static stream& read(stream& in, std::map<k,v>& m){
        k tmp;
        v tmp2;
        typename std::map<k,v>::size_type size;
        m.clear();
        for(bio<typename std::map<k,v>::size_type>::read(in,size);size>0;--size){
            bio<k>::read(in,tmp);
            bio<v>::read(in,tmp2);
            m.emplace(std::move(tmp),std::move(tmp2));
        }
        return in;
    } 
    template<bino::reader stream> inline static std::map<k,v> read(stream& in){
        k tmp;
        v tmp2;
        typename std::map<k,v>::size_type size;
        std::map<k,v> m;
        for(bio<typename std::map<k,v>::size_type>::read(in,size);size>0;--size){
            bio<k>::read(in,tmp);
            bio<v>::read(in,tmp2);
            m.emplace(std::move(tmp),std::move(tmp2));
        }
        return m;
    }
};

#endif