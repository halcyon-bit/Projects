#include "base/base64/base64.h"

#include "gtest/gtest.h"

using halcyon::base::Base64;

class Base64Test : public testing::Test 
{
public:
    static void SetUpTestCase()
    {
        test_str3_ = std::move(std::string(1000000, 'c'));
        test_res3_.reserve(333334 * 4);
        for (size_t i = 0; i < 333333; ++i) {
            test_res3_.append("Y2Nj");
        }
        test_res3_.append("Yw==");

        test_str4_ = std::move(std::string(10000, 'c'));
        test_res4_.reserve(3334 * 4);
        for (size_t i = 0; i < 3333; ++i) {
            test_res4_.append("Y2Nj");
        }
        test_res4_.append("Yw==");
    }

    static void TearDownTestCase()
    {

    }

public:
    static Base64 base64_;
    static std::string test_str1_;
    static std::string test_res1_;
    static std::string test_str2_;
    static std::string test_res2_;
    static std::string test_str3_;
    static std::string test_res3_;
    static std::string test_str4_;
    static std::string test_res4_;
};

Base64 Base64Test::base64_;
std::string Base64Test::test_str1_{ "foahwnxicvhaefaf" };
std::string Base64Test::test_res1_{ "Zm9haHdueGljdmhhZWZhZg==" };
std::string Base64Test::test_str2_{ "35cvawexzcv3546@#" };
std::string Base64Test::test_res2_{ "MzVjdmF3ZXh6Y3YzNTQ2QCM=" };
std::string Base64Test::test_str3_;
std::string Base64Test::test_res3_;
std::string Base64Test::test_str4_;
std::string Base64Test::test_res4_;


TEST_F(Base64Test, base64)
{
    EXPECT_EQ(test_res1_, base64_.encode((const uint8_t*)test_str1_.c_str(), static_cast<int32_t>(test_str1_.length())));
    EXPECT_EQ(test_str1_, base64_.decode(test_res1_.c_str(), static_cast<int32_t>(test_res1_.length())));

    EXPECT_EQ(test_res2_, base64_.encode((const uint8_t*)test_str2_.c_str(), static_cast<int32_t>(test_str2_.length())));
    EXPECT_EQ(test_str2_, base64_.decode(test_res2_.c_str(), static_cast<int32_t>(test_res2_.length())));
}

TEST_F(Base64Test, base64_time)
{
    EXPECT_EQ(test_res3_, base64_.encode((const uint8_t*)test_str3_.c_str(), static_cast<int32_t>(test_str3_.length())));
    EXPECT_EQ(test_str3_, base64_.decode(test_res3_.c_str(), static_cast<int32_t>(test_res3_.length())));
}

TEST_F(Base64Test, base64_effective)
{
    for (size_t i = 0; i < 10000; ++i) {
        EXPECT_EQ(test_res4_, base64_.encode((const uint8_t*)test_str4_.c_str(), static_cast<int32_t>(test_str4_.length())));
        EXPECT_EQ(test_str4_, base64_.decode(test_res4_.c_str(), static_cast<int32_t>(test_res4_.length())));
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
