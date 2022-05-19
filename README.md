Bino - a binary input output library for c++-17 (upwards)

Supported container-types:

    std::vector
    std::map
    std::set
    std::unordered_map
    std::unordered_set
    std::array
    std::basic_string
    std::pair
    std::tuple

In case of vector and array, internal memory-layout is used to speed up reading and writing if possible

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

Reading and writing accept additional parameter of type std::ios_base::openmode:

    bino::write("dst.dat",A,std::ios::out);
    bino::read("dst.dat",A,std::ios::binary);

In case of write, binary and out are activated by default, in case of read, binary and in are activated by default.

Read and write support usage of previously opened fstream-objects, and of- or ifstream-objects.

    std::fstream f("dst.dat",std::ios::out|std::ios::binary);
    bino::write(f,A);

