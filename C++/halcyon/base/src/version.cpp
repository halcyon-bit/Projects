#include "base/common/base_define.h"

BASE_BEGIN_NAMESPACE

#define BASE_VERSION_MAJOR  "1"
#define BASE_VERSION_MINOR  "0"
#define BASE_VERSION_PATCH  "1"

#define BASE_VERSION    "v" BASE_VERSION_MAJOR "." BASE_VERSION_MINOR "." BASE_VERSION_PATCH

const char* version()
{
    return BASE_VERSION;
}

BASE_END_NAMESPACE