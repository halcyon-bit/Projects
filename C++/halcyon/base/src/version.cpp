#include "base/version.h"

#define BASE_VERSION_MAJOR  "1"
#define BASE_VERSION_MINOR  "0"
#define BASE_VERSION_PATCH  "2"

#define BASE_VERSION    "v" BASE_VERSION_MAJOR "." BASE_VERSION_MINOR "." BASE_VERSION_PATCH

const char* halcyon::base::version()
{
    return BASE_VERSION;
}
