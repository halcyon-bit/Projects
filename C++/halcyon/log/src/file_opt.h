#ifndef LOG_FILE_OPT_H
#define LOG_FILE_OPT_H

#include <log/log_define.h>

#include <base/common/base_define.h>

#ifdef USE_HALCYON_STRING_VIEW
#include <base/string/string_view.h>
#define STRING_VIEW_NS  base::
#else
#include <string_view>
#define STRING_VIEW_NS  std::
#endif

LOG_BEGIN_NAMESPACE

namespace file
{
    /*
     * @brief       判断文件夹或文件是否存在
     * @param[in]   文件或文件夹路径
     * @return      是否存在
     */
    bool exists(STRING_VIEW_NS string_view filename);

    /*
     * @brief       创建文件夹
     * @param[in]   文件夹路径(./test/test)
     * @return      是否成功
     * @ps          路径最后不能有 '/' 或 '\\'
     */
    bool createDir(STRING_VIEW_NS string_view dir);

    /*
     * @brief       删除文件夹
     * @param[in]   文件夹路径
     * @return      是否成功
     */
    bool removeDir(STRING_VIEW_NS string_view dir);
}

LOG_END_NAMESPACE

#endif