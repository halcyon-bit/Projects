#include "file_opt.h"

#if defined USE_CPP11 || defined USE_CPP14

#ifdef LINUX
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

#ifdef LINUX
    return access(filename.data(), 0) == 0;
#elif defined WINDOWS
    return _access(filename.data(), 0) == 0;
#endif

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
#ifdef LINUX
            ret = mkdir(tmp.data(), 0777) == 0;
#elif defined WINDOWS
            ret = _mkdir(tmp.data()) == 0;
#endif
        } else if (i == size - 1) {
#ifdef LINUX
            ret = mkdir(dir.data(), 0777) == 0;
#elif defined WINDOWS
            ret = _mkdir(dir.data()) == 0;
#endif
        }
    }
    return ret;
#else
    return std::filesystem::create_directories(dir.data());
#endif
}

#if defined USE_CPP11 || defined USE_CPP14
#ifdef LINUX
static bool remove_dir(STRING_VIEW_NS string_view dir)
{
    DIR* newDir = opendir(dir.data());
    if (nullptr == newDir) {
        return false;
    }

    struct dirent* dirp;
    while ((dirp = readdir(newDir)) != nullptr) {
        if (dirp->d_type == DT_DIR) {
            if (dirp->d_name[0] == '.')
                continue;
            
            std::string path(dir.data());
            path.append("/").append(dirp->d_name);
            if (!remove_dir(path)) {
                return false;
            }
        } else if (dirp->d_type == DT_REG) {
            std::string path(dir.data());
            path.append("/").append(dirp->d_name);
            if (remove(path.c_str()) != 0) {
                return false;
            }
        }
    }
    closedir(newDir);
    return rmdir(dir.data()) == 0;
}
#elif defined WINDOWS
static bool remove_dir(STRING_VIEW_NS string_view dir)
{
    std::string newDir(dir.data());
    newDir.append("\\*.*");

    struct _finddata_t fileInfo;
    intptr_t handle = _findfirst(newDir.c_str(), &fileInfo);
    if (handle == -1) {
        return false;
    }

    // 删除该文件夹下的所有文件(夹)
    do {
        if (fileInfo.attrib & _A_SUBDIR) {
            if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
                continue;

            newDir = dir.data();
            newDir.append("\\").append(fileInfo.name);
            // 文件夹，再次遍历
            if (!remove_dir(newDir)) {
                return false;
            }
        }
        else {
            // 文件
            newDir = dir.data();
            newDir.append("\\").append(fileInfo.name);
            if (remove(newDir.c_str()) != 0) {
                _findclose(handle);
                return false;
            }
        }
    } while (!_findnext(handle, &fileInfo));

    _findclose(handle);
    // 删除空文件夹
    return _rmdir(dir.data()) == 0;
}
#endif
#endif

bool file::removeDir(STRING_VIEW_NS string_view dir)
{
#if defined USE_CPP11 || defined USE_CPP14
    return remove_dir(dir);
#else
    return std::filesystem::remove_all(dir.data());
#endif
}

LOG_END_NAMESPACE