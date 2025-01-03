#include <BaseApp.h> // precompiled

#include <SDK/Proton/MiscUtils.h>
#include <SDK/Proton/FileSystem/FileManager.h>
#include <chrono>
#include <array>
#include <cassert>
#include <format>
#include <regex>
#include <fstream>
#include <algorithm>
#include <sstream>

#pragma warning(disable : 4996)

std::vector<std::string> Utils::StringTokenize(const std::string& text, const std::string& delim)
{
    std::vector<std::string> result;
    if (text.empty())
	{
		return result;
	}

    size_t start = 0, end = 0;
    while (end != std::string::npos) 
	{
        end = text.find(delim, start);
        result.push_back(text.substr(start, (end == std::string::npos) ? std::string::npos: end - start ));
        start = ((end > (std::string::npos - delim.size())) ? std::string::npos :  end + delim.size());
    }

    return result;
}

std::vector<std::string> Utils::SplitString(const std::string& text, const std::string& delim)
{
    std::size_t startPos = 0, endPos, delimiterLength = delim.length();
    std::string token{};
    std::vector<std::string> ret{};

    while ((endPos = text.find(delim, startPos)) != std::string::npos) 
	{
        token = text.substr(startPos, endPos - startPos);
        startPos = endPos + delimiterLength;
        ret.push_back(token);
    }

    ret.push_back(text.substr(startPos));
    return ret;
}

bool Utils::SeparateString(const char str[], int num, char delim, char* pResult) 
{
    size_t sLen = std::strlen(str);
    int l = 0;
    int c = 0;

    for (unsigned int k = 0; str[k] != 0; k++) 
	{
        if (str[k] == delim) 
		{
            l++;
            if (l == num+1)
			{
				break;
			}
            if (k < sLen) 
			{
				c = 0;
			}
        }

        if (str[k] != delim)
		{
			pResult[c++] = str[k];
		}
    }

    pResult[c] = 0;
    if (l < num) 
	{
        pResult[0] = 0;
        return(false);
    }

    return true;
}

std::string Utils::SeparateStringSTL(std::string input, int index, char delim)
{
    if (input.size() > 4048)
	{
		return "";
	}

    char stInput[4048];
    if (SeparateString(input.c_str(), index, delim, stInput))
	{
		return stInput;
	}

    return "";
}

void Utils::StringReplace(const std::string& what, const std::string& with, std::string& in)
{
    size_t pos = 0;
    size_t whatLen = what.length();
    size_t withLen = with.length();

    while ((pos = in.find(what, pos)) != std::string::npos) 
	{
        in.replace(pos, whatLen, with);
        pos += withLen;
    }
}

bool Utils::ContainsGTSwear(const std::string& text) 
{
	std::string test = text;
	std::transform(test.begin(), test.end(), test.begin(), ::tolower);
	std::vector<std::string> slurs = { "faggot", "cunt", "nigger", "blowjob", "porn", "erection", "twat", "masturba", "orgy", "dick", "vagina", "pussy", "penis", "fuck", "fag", "kunt", "kike", "gago", "phuck", "fuk", "fuc", "cock", "cuck", "bitch", "slut", "shore", "shit" };
	for (const auto& slur : slurs) 
	{
		std::string lowercaseSlur = slur;
		std::transform(lowercaseSlur.begin(), lowercaseSlur.end(), lowercaseSlur.begin(), ::tolower);
		
		if (test.find(lowercaseSlur) != std::string::npos)
		{
			return true;
		}
	}

	return false;
}

bool Utils::IsStringNumber(const std::string& str) 
{
	for (char c : str) 
	{
		if (!std::isdigit(c))
		{
			return false;
		}
	}

	return true;
}

bool Utils::IsOnlyAlphabet(const std::string& str)
{
	return str.find_first_not_of("0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm") == std::string::npos;
}

std::string Utils::StringLowercase(std::string what)
{
	std::transform(what.begin(), what.end(), what.begin(), ::tolower);
	return what;
}

std::string Utils::StringUppercase(std::string what)
{
	std::transform(what.begin(), what.end(), what.begin(), ::toupper);
	return what;
}

bool Utils::StringContainsExpressions(const std::string& what)
{
	if (what == ":)" ||
		what == ">:(" ||
		what == ":D" ||
		what == ";)" ||
		what == ":(" ||
		what == ":o" ||
		what == ":O" ||
		what == ":~(" ||
		what == ":'(" ||
		what == ":,(" ||
		what == ":|" ||
		what == ":/" ||
		what == ":," ||
		what == ";-)" ||
		what == ":*")
	{
		return true;
	}

	return false;
}

void Utils::RemoveBadCharacters(std::string& str) 
{
	str.erase(std::remove_if(str.begin(), str.end(), [](char c) 
	{
		return !std::isalnum(c) && c != ' ' && c != '!' && c != '"' && c != '#' &&
			c != '$' && c != '%' && c != '&' && c != '\'' && c != '(' && c != ')' &&
			c != '*' && c != '+' && c != ',' && c != '-' && c != '.' && c != '/' &&
			c != ':' && c != ';' && c != '<' && c != '=' && c != '>' && c != '?' &&
			c != '@' && c != '[' && c != '\\' && c != ']' && c != '^' && c != '_' &&
			c != '`' && c != '{' && c != '}' && c != '~';
	}), str.end());

	str.erase(std::remove(str.begin(), str.end(), '|'), str.end());
}

void Utils::TrimSpacesLeftOfString(std::string& s) 
{
	const char* start = s.c_str();
	const char* end = start + s.size();
	while (start < end && std::isspace(*start))
	{
		start++;
	}

	s.erase(0, start - s.c_str());
}

void Utils::TrimSpacesRightOfString(std::string& s) 
{
	const char* start = s.c_str();
	const char* end = start + s.size();
	const char* last_non_space = end;
	while (last_non_space > start && std::isspace(*(last_non_space - 1)))
	{
		last_non_space--;
	}

	s.erase(last_non_space - s.c_str(), end - last_non_space);
}

void Utils::TrimSpaceBothSidesOfString(std::string& s) 
{
	TrimSpacesLeftOfString(s);
	TrimSpacesRightOfString(s);
}

void Utils::RemoveExtraSpaces(std::string& str) 
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) { return !std::isspace(c); }));
	str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) { return !std::isspace(c); }).base(), str.end());
	auto last = std::unique(str.begin(), str.end(), [](char a, char b) { return std::isspace(a) && std::isspace(b); });
	str.erase(last, str.end());
}

void Utils::RemoveSpaces(std::string& str) 
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

void Utils::FFMFThisString(std::string& str) 
{
	if (str.empty())
	{
		return;
	}

	char arr[2] = { 'f', 'm' };
	for (char& c : str) 
	{
		if (std::isalpha(c)) 
		{
			char let = arr[std::rand() % 2];
			if (std::isupper(c))
			{
				c = std::toupper(let);
			}
			else
			{
				c = let;
			}
		}
	}
}

void Utils::PrepareForInput(std::string& what)
{
	Utils::RemoveBadCharacters(what);
	Utils::TrimSpaceBothSidesOfString(what);
	Utils::RemoveExtraSpaces(what);
}

uint32_t Utils::HashString(const void* pMem, uint32_t dataLength) 
{
    if (!pMem) 
	{
		return 0;
	}

    uint32_t hash = 0x55555555;

    for (uintmax_t i = 0; i < dataLength; ++i)
	{
		hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(pMem)[i];
	}

    return hash;
}

int32_t Utils::GenerateUniqueTag(const std::string& name, const std::string& relativeId, const std::string& key) 
{
	std::hash<std::string> hasher;
	size_t hashValue = hasher(name + relativeId + key + std::to_string(time(NULL)));
	int32_t uniqueNumber = (hashValue % 900) + 100;

	if (uniqueNumber < 100) 
	{
		uniqueNumber += 100;
	}
	
	if (uniqueNumber > 999)
	{
		uniqueNumber %= 900 + 100;
	}
		
	return uniqueNumber;
}

std::string Utils::ResolveIPAddress(const uint32_t& ip) 
{
    std::ostringstream oss;
    oss  << ((ip >> 24) & 0xFF) << '.' << ((ip >> 16) & 0xFF) << '.' << ((ip >> 8) & 0xFF) << '.' << (ip & 0xFF);
    return oss.str();
}

std::string Utils::NormalizeString(const std::string& str) 
{
	std::string normalizedStr = str;
	std::transform(normalizedStr.begin(), normalizedStr.end(), normalizedStr.begin(), [](unsigned char c) { return std::tolower(c); });
	return normalizedStr;
}

std::string Utils::GetLogonVariantString(float gVer) 
{
	//logon packets for different gt client versions(if you know missing ones, specify from which to which version is it and the variant name and we will add it
    if (gVer >= 2.982f) 
	{
		return "OnSuperMainStartAcceptLogonHrdxs47254722215a";
	}
    else if (gVer > 2.47f) 
	{
		return "OnSuperMainStartAcceptLogonFB211131ddf";
	}
    else if (gVer > 2.45f) 
	{
		return "OnSuperMainStartAcceptLogonFB211131dd";
	}
    else if (gVer > 2.44f) 
	{
		return "OnSuperMainStartAcceptLogonFB211131d";
	}
    else if (gVer > 2.37f) 
	{
		return "OnSuperMainStartAcceptLogonFB211131";
	}
    else if (gVer > 2.35f) 
	{
		return "OnSuperMainStartAcceptLogonFB21113";
	}
    else if (gVer > 2.32f) 
	{
		return "OnSuperMainStartAcceptLogonFB2111";
	}
    else if (gVer > 2.30f) 
	{
		return "OnSuperMainStartAcceptLogonFB211";
	}
    else if (gVer > 2.27f) 
	{
		return "OnSuperMainStartAcceptLogonFB21";
	}
    else if (gVer > 2.22f) 
	{
		return "OnSuperMainStartAcceptLogonFB2";
	}
    else if (gVer > 2.17f) 
	{
		return "OnSuperMainStartAcceptLogonFB";
	}
    else if (gVer > 2.16f) 
	{
		return "OnSuperMainStartAcceptLogonF";
	}
    else if (gVer > 2.13f) 
	{
		return "OnSuperMainStartAcceptLogon";
	}
    else if (gVer > 1.95f) 
	{
		return "OnAcceptLogon";
	}
    else if (gVer > 1.93f) 
	{
		return "OnFirstLogonAccepted";
	}
    else if (gVer > 1.89f) 
	{
		return "OnLogonAccepted";
	}
	else if (gVer == 0.00f)
	{
		return "OnSuperMainStartAcceptLogonHrdxs47254722215a";
	}

    return "OnInitialLogonAccepted";
}

bool Utils::SaveToFile(const std::string& fName, const std::string& content) 
{
    std::ofstream outputFile(fName);
    if (!outputFile.is_open())
	{
		return false;
	}

    outputFile << content;
    outputFile.close();
    return true;
}

unsigned int Utils::GetHashOfFile(const std::string& fName) 
{
	unsigned int hash = 0;
	int size = 0;
	uint8_t *pData = GetFileManager()->Get(fName, &size, false);

	if (pData == NULL)
	{
		//no file, no hash
		return 0;
	}

	hash = HashString((char*)pData, size);
	nova_delete_arr(pData);
	return hash;
}

void MemorySerialize(std::string& num, uint8_t* pMem, int& offsetInOut, bool bWriteToMem) 
{
	uint16_t len = 0;
	assert(num.length() < 1024 * 64);
	if (bWriteToMem) 
	{
		len = (uint16_t) num.length();
		memcpy(&pMem[offsetInOut], &len, sizeof(len));
		offsetInOut += sizeof(len);
		memcpy(&pMem[offsetInOut], num.c_str(), len);
	} 
	else 
	{
		memcpy(&len, &pMem[offsetInOut], sizeof(len));
		offsetInOut += sizeof(len);
		num.resize(len);
		memcpy((void*)num.c_str(), &pMem[offsetInOut], len);
	}

	offsetInOut += len;
}

size_t CalculateMemoryItemNameEncryptedSize(const std::string& num, int cryptID, const char* secretCode)
{
	uint16_t len = static_cast<uint16_t>(num.length());
	int codeLen = static_cast<int>(strlen(secretCode));
	cryptID = cryptID % codeLen;

	size_t strLen = sizeof(uint16_t);

	for (int i = 0; i < len; i++)
	{
		uint8_t b = static_cast<uint8_t>(num.c_str()[i]);
		b = b ^ secretCode[cryptID++];
		if (cryptID >= codeLen)
		{
			cryptID = 0;
		}

		strLen += 1;
	}

	return strLen;
}

void MemorySerializeStringEncrypted(std::string& num, uint8_t* pMem, int& offsetInOut, bool bWriteToMem, int cryptID, const char* secretCode) 
{
	uint16_t len;
	int codeLen = (int)strlen(secretCode);
	cryptID = cryptID % codeLen;

	if (bWriteToMem) 
	{
		len = (uint16_t) num.length();
		memcpy(&pMem[offsetInOut], &len, sizeof(len));
		offsetInOut += sizeof(len);

		for (int i = 0; i < len; i++) 
		{
			uint8_t b = (uint8_t)num.c_str()[i];
			b = b ^ secretCode[cryptID++];
			if (cryptID >= codeLen)
			{
				cryptID = 0;
			}

			pMem[offsetInOut++] = b;
		}
	} 
	else 
	{
		memcpy(&len, &pMem[offsetInOut], sizeof(len));
		offsetInOut += sizeof(len);
		num.resize(len);

		for(int i = 0; i < len; i++) 
		{
			uint8_t b = pMem[offsetInOut++];
			num[i] = b ^ secretCode[cryptID++];

			if (cryptID >= codeLen)
			{
				cryptID = 0;
			}
		}
	}
}

std::string EncryptItemNameString(const std::string& num, int cryptID, const char* secretCode)
{
	std::string encrypted;
	int codeLen = static_cast<int>(strlen(secretCode));
	cryptID = cryptID % codeLen;

	for (size_t i = 0; i < num.length(); i++)
	{
		uint8_t b = static_cast<uint8_t>(num[i]);
		b = b ^ secretCode[cryptID++];
		if (cryptID >= codeLen)
		{
			cryptID = 0;
		}

		encrypted.push_back(static_cast<char>(b));
	}

	return encrypted;
}


bool MemorySerializeStringLarge(std::string& num, uint8_t* pMem, int& offsetInOut, bool bWriteToMem, uint32_t maxBytesInPacket) 
{
	uint32_t len;
	if (bWriteToMem) 
	{
		len = (uint32_t)num.length();
		if (maxBytesInPacket != 0 && len > (maxBytesInPacket - 4))
		{
			return false;
		}

		memcpy(&pMem[offsetInOut], &len, sizeof(len));
		offsetInOut += sizeof(len);
		memcpy(&pMem[offsetInOut], num.c_str(), len);
	} 
	else 
	{
		memcpy(&len, &pMem[offsetInOut], sizeof(len));
		offsetInOut += sizeof(len);
		num.resize(len);
		memcpy((void*)num.c_str(), &pMem[offsetInOut], len);
	}

	offsetInOut += len;
	return true;
}

std::string Utils::GetRandomPart(const int& index)
{
	switch (index) 
	{
	    case 0:
			return "Death";
		case 1:
			return "Gar";
		case 2:
			return "Lite";
		case 3:
			return "Wave";
		case 4:
			return "Rat";
		case 5:
			return "Zero";
		case 6:
			return "Grow";
		case 7:
			return "Mouse";
		case 8:
			return "Luck";
		case 9:
			return "Bucks";
		case 10:
			return "Cry";
		case 11:
			return "Board";
		case 12:
			return "You";
		case 13:
			return "Flash";
		case 14:
			return "Banana";
		case 15:
			return "Einst";
		case 16:
			return "Azure";
		case 17:
			return "Grip";
		case 18:
			return "Punch";
		case 19:
			return "Laugh";
		case 20:
			return "Solid";
		case 21:
			return "Snake";
		case 22:
			return "Duck";
		case 23:
			return "Len";
		case 24:
			return "Sickle";
		case 25:
			return "Bill";
		case 26:
			return "Smile";
		case 27:
			return "Joy";
		case 28:
			return "Shiny";
		case 29:
			return "Watch";
		case 30:
			return "Pie";
		case 31:
			return "Dawn";
		case 32:
			return "Brave";
		case 33:
			return "Head";
		case 34:
			return "Fairy";
		case 35:
			return "Smell";
		case 36:
			return "Dar";
		case 37:
			return "Tiny";
		case 38:
			return "Krazy";
		case 39:
			return "Bob";
		case 40:
			return "Burp";
		case 41:
			return "Tickle";
		case 42:
			return "Wiggle";
		case 43:
			return "Squish";
		case 44:
			return "Fun";
		case 45:
			return "Good";
		case 46:
			return "Bad";
		case 47:
			return "Fire";
		case 48:
			return "Cake";
		default:
			return "Tor";
	}

	return "Tor";
}