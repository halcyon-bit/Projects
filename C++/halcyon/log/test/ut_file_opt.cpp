#include "file_opt.h"

#include "base/time/timestamp.h"

#include "gtest/gtest.h"

using namespace halcyon;

TEST(FileOptTest, exists)
{
    bool ret = log::file::exists("./123");
    EXPECT_EQ(ret, false);

    ret = log::file::exists("./123.txt");
    EXPECT_EQ(ret, false);

    ret = log::file::exists("../../bin");
    EXPECT_EQ(ret, true);
    ret = log::file::exists("../../bin/");
    EXPECT_EQ(ret, true);

    ret = log::file::exists("../../test/ut_file_opt.cpp");
    EXPECT_EQ(ret, true);
}

TEST(FileOptTest, dir)
{
    // create dir
    {
        bool ret = log::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::createDir("./test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, true);
    }

    base::sleep(5000);

    // remove dir
    {
        bool ret = log::file::removeDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::removeDir("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, false);
    }
/*
    // remove dir
    {
        bool ret = log::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::removeDir("./test");
        EXPECT_EQ(ret, true);
    }
    */
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
