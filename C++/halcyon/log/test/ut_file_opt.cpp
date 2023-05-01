#define _CRT_SECURE_NO_WARNINGS
#include "file_opt.h"

#include "base/time/timestamp.h"

#include "gtest/gtest.h"

#include <fcntl.h>

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

    base::sleep(500);

    // remove dir
    {
        bool ret = log::file::removeDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::removeDir("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = log::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = log::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = log::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        std::string filePath{ "./test/test/" };
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
#ifdef WINDOWS
            FILE* fp = fopen(tmp.c_str(), "w");
            fclose(fp);
#elif defined LINUX
            int fp = open(tmp.c_str(), O_CREAT);
            close(fp);
#endif
        }

        ret = log::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = log::file::removeDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = log::file::exists("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = log::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        std::string filePath{ "./test/" };
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
#ifdef WINDOWS
            FILE* fp = fopen(tmp.c_str(), "w");
            fclose(fp);
#elif defined LINUX
            int fp = open(tmp.c_str(), O_CREAT);
            close(fp);
#endif
        }

        filePath.append("test/");
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
#ifdef WINDOWS
            FILE* fp = fopen(tmp.c_str(), "w");
            fclose(fp);
#elif defined LINUX
            int fp = open(tmp.c_str(), O_CREAT);
            close(fp);
#endif
        }

        ret = log::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, true);
        ret = log::file::exists("./test/9.txt");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = log::file::removeDir("./test/");
        EXPECT_EQ(ret, true);

        ret = log::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, false);
        ret = log::file::exists("./test/9.txt");
        EXPECT_EQ(ret, false);
        ret = log::file::exists("./test");
        EXPECT_EQ(ret, false);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
