#include <gflags/gflags.h>
#include <cstdio>
#include <iostream>
DECLARE_int32(varint33);
DEFINE_int32(varint, 33 , "fuck intergr");
int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << FLAGS_varint << std::endl;
    std::cout << FLAGS_varint33 << std::endl;
    return 0;
}
