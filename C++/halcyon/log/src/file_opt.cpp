#include "file_opt.h"

#if defined USE_CPP11 || defined USE_CPP14

#ifdef LINUX
//#include <sys/io.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif defined WINDOWS
#include <io.h>
#include <direct.h>
#endif

#else
#include <filesystem>  // c++17
#endif

LOG_BEGIN_NAMESPACE

bool file::exists(STRING_VIEW_NS string_view filename)
{
#if defined USE_CPP11 || defined USE_CPP14
    return access(filename.data(), 0) == 0;
#else
    return std::filesystem::exists(filename.data());
#endif
}

bool file::createDir(STRING_VIEW_NS string_view dir)
{
#if defined USE_CPP11 || defined USE_CPP14
    bool ret = false;
    size_t start = dir.find("/");
    if (start == -1) {
        start = dir.find("\\");
    }
    if (start == -1) {
        return false;
    }

    size_t size = dir.size();
    for (size_t i = start + 1; i < size; ++i) {
        if (dir[i] == '\\' || dir[i] == '/') {
            std::string tmp(dir.substr(0, i).data(), i);
            ret = mkdir(tmp.data(), 0777) == 0;
        } else if (i == size - 1) {
            ret = mkdir(dir.data(), 0777) == 0;
        }
    }
    return ret;
#else
    return std::filesystem::create_directories(dir.data());
#endif
}
// #include <stdio.h>
// int deleteAll(char* directory) {
//     DIR* dir = NULL;
//     char dpath[PATH_MAX];
//     char fpath[PATH_MAX];
//     char answer;
//     _Bool delete = 1;
//     if ((dir = opendir(directory)) == NULL) {
//         printf("opendir: No such a directory: %s\n", directory);
//         return 1;
//     }
//     struct dirent* dirp;
//     while ((dirp = readdir(dir)) != NULL) {
//         if (dirp->d_type == DT_DIR) {
//             if (dirp->d_name[0] == '.')
//                 continue;
//             deleteAll(dpath);
//         }
//         if (dirp->d_type == DT_REG) {
//             snprintf(fpath, (size_t)PATH_MAX, "%s/%s", directory, dirp->d_name);
//             if (check) {
//                 printf("\033[1;31mAre you sure to delete file \"%s\"?(y/n)\033[0m\n", dirp->d_name);
//                 answer = getchar();
//                 getchar();
//                 if (answer == 'n')
//                     delete = 0;
//                 else
//                     delete = 1;
// }
//             if (delete) {
//                 if (!remove(fpath))
//                     printf("\033[32mFile removed:%s\033[0m\n", dirp->d_name);
//                 else
//                     perror("remove");
//             }
//         }
//     }
//     closedir(dir);
//     if (!rmdir(directory))
//         printf("\033[32mDirectory removed:%s\033[0m\n", directory);
//     return 0;
// }

bool file::removeDir(STRING_VIEW_NS string_view dir)
{
#if defined USE_CPP11 || defined USE_CPP14
    return remove(dir.data()) == 0;
    //return _rmdir(dir.data()) == 0;
    bool ret = false;
    size_t end = dir.find("/");
    if (end == -1) {
        end = dir.find("\\");
    }
    if (end == -1) {
        return false;
    }

    int32_t size = (int32_t)dir.size();
    for (size_t i = size - 1; i > end; --i) {
        if (dir[i] == '\\' || dir[i] == '/') {
            std::string tmp(dir.substr(0, i).data(), i);
            //ret = _rmdir(tmp.data()) == 0;
        }
    }
    return ret;
#else
    return std::filesystem::remove_all(dir.data());
#endif
}

LOG_END_NAMESPACE