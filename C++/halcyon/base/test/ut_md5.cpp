#include "base/md5/md5.h"
#include "gtest/gtest.h"

using namespace halcyon::base;

TEST(MD5Test, md5_base)
{
    std::string str1{ "abc" }, res1{ "900150983cd24fb0d6963f7d28e17f72" };
    EXPECT_EQ(res1, MD5(str1).toString());

    MD5 md5;
    std::string str2{ "" }, res2{ "d41d8cd98f00b204e9800998ecf8427e" };

    md5.update(str2);
    EXPECT_EQ(res2, md5.toString());

    std::string str3{ "abcdefghijklmnopqrstuvwxyz" }, res3{ "c3fcd3d76192e4007dfb496cca67e13b" };
    md5.reset();
    md5.update(str3);
    EXPECT_EQ(res3, md5.toString());

    std::string str4{ "awf234zxv234123548@#43as4" }, res4{ "0c3c2778d43b083f11d342eb6bdf8ce2" };
    md5.reset();
    md5.update(str4);
    EXPECT_EQ(res4, md5.toString());
}

TEST(MD5Test, md5_effective)
{
    MD5 md5;
    std::string str4{ "awozxohvaerughaphpuhizupgisdgfipuawefgbipuabiughiurgbnzvpziuhsgiuahtweignapighr[eiaohgaiuhga;jfzubvu" };

    for (int32_t i = 0; i < 1000000; ++i) {
        md5.reset();
        md5.update(str4);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
