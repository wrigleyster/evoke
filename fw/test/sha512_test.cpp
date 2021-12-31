#include <boost/test/unit_test.hpp>
#include "fw/sha512.h"
#include <fstream>

BOOST_AUTO_TEST_CASE(sha512_win32)
{
    // Open in binary mode to have consistent line endings in win and linux.
    std::ofstream testFile("test_src.cpp", std::ios::binary);
    testFile << "void main()\x0a";
    testFile << "{\x0a";
    testFile << "    return;\x0a";
    testFile << "}\x0a";
    testFile.close();
    // Hash generated on windows with >certutil -hashfile "test_src.cpp" SHA512
    // df76ebfae227441cf4ba3e1b5cf4b4b0b0d73a384b5d633116c5eeb7fd4333fb6d8a6e83e5488ad1b61fd2e20efe974e42ed32da2438617644c9687e24208ce2
    std::array<uint8_t, 64> hash{ 
     0xdf,0x76,0xeb,0xfa,0xe2,0x27,0x44,0x1c,0xf4,0xba,0x3e,0x1b,0x5c,0xf4,0xb4,0xb0,0xb0,0xd7,0x3a,0x38,0x4b,0x5d,0x63,0x31,0x16,0xc5,
     0xee,0xb7,0xfd,0x43,0x33,0xfb,0x6d,0x8a,0x6e,0x83,0xe5,0x48,0x8a,0xd1,0xb6,0x1f,0xd2,0xe2,0x0e,0xfe,0x97,0x4e,0x42,0xed,0x32,0xda,
     0x24,0x38,0x61,0x76,0x44,0xc9,0x68,0x7e,0x24,0x20,0x8c,0xe2};
    BOOST_TEST(sha512(fs::path("./test_src.cpp")) == hash);
    fs::remove("test_src.cpp");
}