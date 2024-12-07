#pragma once
#include <cstring>
#include <string>
#include <random>
#include <vector>
#include <string_view>
#include <cmath>
#include <cstdlib>

#define MAKE_RGB(r, g, b) (((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24))
#define MAKE_RGBA(r, g, b, a) (((uint32_t)(r) << 8) + ((uint32_t)(g) << 16) + ((uint32_t)(b) << 24) + ((uint32_t)(a)))
#define FROM_RGBA(r, g, b, a) ((uint32_t)(a) | (uint32_t)(r) << 8 | (uint32_t)(g) << 16 | (uint32_t)(b) << 24)

#define GET_BLUE(p)     ((p) >> 24)
#define GET_GREEN(p)    (((p) & 0x00FF0000) >> 16)
#define GET_RED(p)      (((p) & 0x0000FF00) >>  8)
#define GET_ALPHA(p)    ((p) & 0x000000FF)

constexpr uint64_t HashStringFNV(const std::string_view& input) 
{
    if (input.empty())
    {
        return 0;
    }

    uint64_t hash = 0;
    for (size_t i = 0; i < input.size(); i++) 
    {
        hash += input[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    hash ^= (hash >> 23);
    hash += (hash << 27);

    return hash;
}
constexpr uint64_t operator "" _FNV(const char* str, size_t len) 
{
    return HashStringFNV(std::string_view(str, len));
}

class Utils
{
public:
    static std::vector<std::string> StringTokenize(const std::string& text, const std::string& delim = "|");
    static std::vector<std::string> SplitString(const std::string& text, const std::string& delim);
    static bool SeparateString (const char str[], int num, char delim, char* pResult);
    static std::string SeparateStringSTL(std::string input, int index, char delim);
    static void StringReplace(const std::string& what, const std::string& with, std::string& in);
    static bool ContainsGTSwear(const std::string& text);
    static bool IsStringNumber(const std::string& str);
    static bool IsOnlyAlphabet(const std::string& str);
    static std::string StringLowercase(std::string what);
    static std::string StringUppercase(std::string what);
    static bool StringContainsExpressions(const std::string& what);

	static void RemoveBadCharacters(std::string& str);
	static void TrimSpacesLeftOfString(std::string& s);
	static void TrimSpacesRightOfString(std::string& s);
    static void TrimSpaceBothSidesOfString(std::string& s);
    static void RemoveExtraSpaces(std::string& str);
    static void RemoveSpaces(std::string& str);
    static void FFMFThisString(std::string& str);
    static void PrepareForInput(std::string& what);

    static uint32_t HashString(const void* pMem, uint32_t dataLength);
    static int32_t GenerateUniqueTag(const std::string& name, const std::string& relativeId, const std::string& key);
    static std::string ResolveIPAddress(const uint32_t& ip);
    static std::string NormalizeString(const std::string& str);

    static std::string GetRandomPart(const int& index);
    static std::string GetLogonVariantString(float gVer);

    static bool SaveToFile(const std::string& fName, const std::string& content);
    static unsigned int GetHashOfFile(const std::string& fName);
};

class Randomizer 
{
public:
    template<typename T> static T Get(T min, T max) 
    {
        static std::mt19937 rng{ std::random_device()() };
        if constexpr (std::is_integral<T>::value) 
        {
            std::uniform_int_distribution<T> distrib(min, max);
            return distrib(rng);
        }
        else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) 
        {
            std::uniform_real_distribution distrib(min, max);
            return static_cast<T>(distrib(rng));
        }

        return 0;
    }

    template<typename T> static T Get() 
    {
        static std::mt19937 rng{ std::random_device()() };

        if constexpr (std::is_integral<T>::value) 
        {
            std::uniform_int_distribution<T> distrib;
            return distrib(rng);
        }
        else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) 
        {
            std::uniform_real_distribution distrib;
            return static_cast<T>(distrib(rng));
        }

        return 0;
    }
};

void MemorySerialize(std::string& num, uint8_t* pMem, int &offsetInOut, bool bWriteToMem);
template <typename T> void MemorySerializeRaw(T& var, uint8_t* pMem, int& offsetInOut, bool bWriteToMem)
{
    size_t sizeBytes = sizeof(T);
    if (sizeBytes == 0)
    {
        return;
    }

    if (bWriteToMem)
    {
        memcpy(&pMem[offsetInOut], &var, sizeBytes);
    }
    else
    {
        memcpy(&var, &pMem[offsetInOut], sizeBytes);
    }

    offsetInOut += (int)sizeBytes;
}

bool MemorySerializeStringLarge(std::string& num, uint8_t* pMem, int &offsetInOut, bool bWriteToMem, uint32_t maxBytesInPacket = 0);
void MemorySerializeStringEncrypted(std::string& num, uint8_t* pMem, int& offsetInOut, bool bWriteToMem, int cryptID, const char* secretCode);

static std::string EncryptItemNameString(const std::string& num, const uint32_t& cryptID)
{
    constexpr std::string_view key{ "PBG892FXX982ABC*" };
    std::string ret(num.size(), 0);

    for (uint32_t i = 0; i < num.size(); i++)
    {
        ret[i] = static_cast<char>(num[i] ^ key[(i + cryptID) % key.size()]);
    }

    return ret;
}

static std::string base64_encode(const std::string& input)
{
    static constexpr char sEncodingTable[] =
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    size_t in_len = input.size();
    size_t out_len = 4 * ((in_len + 2) / 3);
    std::string ret(out_len, '\0');
    size_t i;
    char* p = &ret[0];

    for (i = 0; i < in_len - 2; i += 3)
    {
        *p++ = sEncodingTable[(input[i] >> 2) & 0x3F];
        *p++ = sEncodingTable[((input[i] & 0x3) << 4) | ((int)(input[i + 1] & 0xF0) >> 4)];
        *p++ = sEncodingTable[((input[i + 1] & 0xF) << 2) | ((int)(input[i + 2] & 0xC0) >> 6)];
        *p++ = sEncodingTable[input[i + 2] & 0x3F];
    }

    if (i < in_len)
    {
        *p++ = sEncodingTable[(input[i] >> 2) & 0x3F];
        if (i == (in_len - 1))
        {
            *p++ = sEncodingTable[((input[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else
        {
            *p++ = sEncodingTable[((input[i] & 0x3) << 4) | ((int)(input[i + 1] & 0xF0) >> 4)];
            *p++ = sEncodingTable[((input[i + 1] & 0xF) << 2)];
        }

        *p++ = '=';
    }

    return ret;
}

static std::string base64_decode(const std::string& input)
{
    static constexpr unsigned char sDecodingTable[] =
    {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 0,  64, 64,
        64, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
    };

    size_t in_len = input.size();
    if (in_len % 4 != 0)
    {
        throw std::runtime_error("Invalid Base64 input length");
    }

    size_t out_len = in_len / 4 * 3;
    if (input[in_len - 1] == '=') 
    {
        out_len--;
    }

    if (input[in_len - 2] == '=') 
    {
        out_len--;
    }

    std::string ret(out_len, '\0');
    size_t i;
    unsigned char* p = reinterpret_cast<unsigned char*>(&ret[0]);

    for (i = 0; i < in_len; i += 4)
    {
        uint32_t n = sDecodingTable[input[i]] << 18 | sDecodingTable[input[i + 1]] << 12 | sDecodingTable[input[i + 2]] << 6 | sDecodingTable[input[i + 3]];
        *p++ = (n >> 16) & 0xFF;

        if (input[i + 2] != '=') 
        {
            *p++ = (n >> 8) & 0xFF;
        }

        if (input[i + 3] != '=') 
        {
            *p++ = n & 0xFF;
        }
    }

    return ret;
}