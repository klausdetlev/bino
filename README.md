Bino - a binary input output library for c++-17 (upwards)

This is a header-only c++-library. Use it by including it into your header. No linking- or compiler-flags needed

Supported container-types:

    std::vector
    std::list
    std::map
    std::set
    std::unordered_map
    std::unordered_set
    std::array
    std::basic_string
    std::pair
    std::tuple

Usage:

Given an object A of type T.
1. Writing:

        bino::write("dst.dat",A);
        bino::write<T>("dst.dat",A);

    Template type is deduced if necessary

2. Reading:

        T A;
        bino::read("src.dat",A);
        bino::read<T>("src.dat",A);

        T A = bino::read<T>("src.dat");

In case of vector and array, internal memory-layout is used to speed up reading and writing if possible.

Reading and writing accept additional parameter of type std::ios_base::openmode:

    bino::write("dst.dat",A,std::ios::out);
    bino::read("dst.dat",A,std::ios::binary);

In case of write, std::ios::binary and std::ios::out are activated by default, in case of read, std::ios::binary and std::ios::in are activated by default.

Read and write support usage of previously opened fstream-objects, and of- or ifstream-objects.

    std::fstream f("dst.dat",std::ios::out|std::ios::binary);
    bino::write(f,A);

    std::ofstream out("dst.dat",std::ios::out|std::ios::binary);
    bino::write(out,A);

    std::ifstream in("src.dat",std::ios::in|std::ios::binary);
    A = bino::read<T>(in);

Extensible format via specialization of struct bio:

    template<typename t> struct bio{
        template<bino::writer stream> inline static stream& write(stream&, const t&);
        template<bino::reader stream> inline static stream& read(stream&, t&);
        template<bino::reader stream> inline static t read(stream&);
    };

With concepts

    template<typename t> concept writer = std::is_same<t,std::fstream>::value || std::is_same<t,std::ofstream>;
    template<typename t> concept reader = std::is_same<t,std::fstream>::value || std::is_same<t,std::ifstream>;

Example for bio<std::vector<t>>:

    template<bino::pod t> struct bio<std::vector<t>>{
        template<bino::writer stream> inline static stream& write(stream& out, const std::vector<t>& v){
            bino::write<typename std::vector<t>::size_type>(out,v.size());
            out.write(reinterpret_cast<const char*>(v.data()),sizeof(t)*v.size());
            return out;
        }
        template<bino::reader stream> inline static stream& read(stream& in, std::vector<t>& v){
            v.resize(bino::read<typename std::vector<t>::size_type>(in));
            in.read(reinterpret_cast<char*>(v.data()),v.size()*sizeof(t));
            return in;
        }
        template<bino::reader stream> inline static std::vector<t> read(stream& in){
            std::vector<t> v(bino::read<typename std::vector<t>::size_type>(in));
            in.read(reinterpret_cast<char*>(v.data()),sizeof(t)*v.size());
            return v;
        }
    };

    template<bino::nonpod t> struct bio<std::vector<t>>{
        template<bino::writer stream> inline static stream& write(stream& out, const std::vector<t>& v){
            bino::write<typename std::vector<t>::size_type>(out,v.size());
            for(typename std::vector<t>::iterator i=v.begin();i!=v.end();++i) bino::write<t>(out,*i);
            return out;
        }
        template<bino::reader stream> inline static stream& read(stream& in, std::vector<t>& v){
            v.resize(bino::read<typename std::vector<t>::size_type>(in));
            for(typename std::vector<t>::iterator i=v.begin();i!=v.end();++i) bino::read<t>(in,*i);
            return in;
        }
        template<bino::reader stream> inline static std::vector<t> read(stream& in){
            std::vector<t> v(bino::read<typename std::vector<t>::size_type>(in));
            for(typename std::vector<t>::iterator i=v.begin();i!=v.end();++i) bino::read<t>(in,*i);
            return v;
        }
    };

With concepts

    template<typename t> concept pod = std::is_trivially_copyable<t>::value;
    template<typename t> concept nonpod = !std::is_triviallay_copyable<t>::value;

In case of a vector containing trivially_copyable objects, one may take advantage of it's linear memory layout and dump the complete strip of memory at once into the stream.

The concepts allow easy distinction between those layouts.