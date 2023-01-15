#include <base/base64/base64.h>

BASE_BEGIN_NAMESPACE

static const std::string kBase64Table{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
static constexpr char kBase64Pad_{ '=' };

BASE_END_NAMESPACE

using namespace halcyon::base;

std::string Base64::encode(const uint8_t* str, int32_t length)
{
    std::string result;
    const uint8_t* current{ str };
    while (length > 2) {
        result += kBase64Table[current[0] >> 2];
        result += kBase64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
        result += kBase64Table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
        result += kBase64Table[current[2] & 0x3f];

        current += 3;
        length -= 3;
    }

    if (length > 0) {
        result += kBase64Table[current[0] >> 2];
        if (length % 3 == 1) {
            result += kBase64Table[(current[0] & 0x03) << 4];
            result += "==";
        }
        else if (length % 3 == 2) {
            result += kBase64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
            result += kBase64Table[(current[1] & 0x0f) << 2];
            result += "=";
        }
    }
    return result;
}

std::string Base64::decode(const char* str, int32_t length)
{
    // 解码表
    const char kDecodeTable[] = {
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
        -2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
        -2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
    };

    int32_t bin{ 0 }, i{ 0 };
    std::string result;
    const char* current{ str };
    char ch;
    while ((ch = *current++) != '\0' && length-- > 0) {
        if (ch == kBase64Pad_) { // 当前一个字符是“=”号
            /*
            先说明一个概念：在解码时，4个字符为一组进行一轮字符匹配。
            两个条件：
                1、如果某一轮匹配的第二个是“=”且第三个字符不是“=”，说明这个带解析字符串不合法，直接返回空
                2、如果当前“=”不是第二个字符，且后面的字符只包含空白符，则说明这个这个条件合法，可以继续。
            */
            if (*current != '=' && (i % 4) == 1) {
                return "";
            }
            continue;
        }
        ch = kDecodeTable[ch];
        // 这个很重要，用来过滤所有不合法的字符
        if (ch < 0) { /* a space or some other separator character, we simply skip over */
            continue;
        }

        switch (i % 4) {
        case 0:
            bin = ch << 2;
            break;
        case 1:
            bin |= ch >> 4;
            result += bin;
            bin = (ch & 0x0f) << 4;
            break;
        case 2:
            bin |= ch >> 2;
            result += bin;
            bin = (ch & 0x03) << 6;
            break;
        case 3:
            bin |= ch;
            result += bin;
            break;
        }
        i++;
    }
    return result;
}
