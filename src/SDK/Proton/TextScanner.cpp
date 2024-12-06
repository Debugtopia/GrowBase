#include <BaseApp.h> // precompiled

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/FileSystem/FileManager.h>
#include <SDK/Proton/MiscUtils.h>
#pragma warning(disable : 4996)

TextScanner::TextScanner() : m_lastLine(0) 
{
    //
}

TextScanner::TextScanner(const char* pCharArray) : m_lastLine(0) 
{
	SetupFromMemoryAddress(pCharArray);
}

TextScanner::TextScanner(const std::string& fName) : m_lastLine(0) 
{
	LoadFile(fName);
}

bool TextScanner::LoadFile(const std::string& fName)
{
	Kill();

	FileInstance f(fName);
	if (!f.IsLoaded()) 
	{
		return false;
	}

	return SetupFromMemoryAddress(f.GetAsChars());
}

bool TextScanner::SetupFromMemoryAddress(const char* pCharArray)
{
	m_lines = Utils::StringTokenize(pCharArray, "\n");
	for (unsigned int i = 0; i < m_lines.size(); i++)
	{
		Utils::StringReplace("\r", "", m_lines[i]);
	}

	return true;
}

bool TextScanner::SetupFromMemoryAddressRaw(const char* pCharArray, int size)
{
	m_lines = Utils::StringTokenize(pCharArray, "\n");
	return true;
}

std::string TextScanner::GetParmString(std::string label, int index, std::string token)
{
	if (m_lines.empty())
	{
		return "";
	}

	for (unsigned int i = 0; i < m_lines.size(); i++) 
	{
		if (m_lines[i].empty()) 
		{
			continue;
		}

		std::vector<std::string> line = Utils::StringTokenize(m_lines[i], token);
		if (line.empty() || index < 0 || index > line.size())
		{
			continue;
		}

		if (line[0] == label)
		{
			return line[index];
		}

	}

	return "";
}
int TextScanner::GetParmInt(std::string label, int index, std::string token) 
{
	return std::atoi(GetParmString(label, index, token).c_str());
}

float TextScanner::GetParmFloat(std::string label, int index, std::string token) 
{
	return (float)std::atof(GetParmString(label, index, token).c_str());
}

TextScanner::~TextScanner()
{
	Kill();
}

void TextScanner::Kill()
{
	m_lines.clear();
	m_lastLine = 0;
}

std::string TextScanner::GetMultipleLineStrings(std::string label,std::string token)
{
	for (unsigned int i=m_lastLine; i < m_lines.size(); i++) 
	{
		if (m_lines[i].empty()) 
		{
			continue;
		}

		std::vector<std::string> line = Utils::StringTokenize(m_lines[i], token);
		if (line[0] == label) 
		{
			m_lastLine = i+1;
			return m_lines[i];
		}
	}

	m_lastLine = 0;
	return "";
}

std::string TextScanner::GetLine(int lineNum)
{
	return ((int)m_lines.size() > lineNum && lineNum >= 0) ? m_lines[lineNum] : "";
}

std::string TextScanner::GetAllRaw()
{
	std::string s;
	for (unsigned int i = 0; i < m_lines.size(); i++)
	{
		s += m_lines[i] + "\n";
	}

	return s;
}

std::string TextScanner::GetParmStringFromLine(int lineNum, int index, std::string token)
{
	if (!(lineNum >= 0 && (uint32_t)lineNum < m_lines.size())) 
	{
		return "";
	}
	if (token.size() != 1)
	{
		return "";
	}

	return Utils::SeparateStringSTL(m_lines[lineNum], index, token[0]);
}

int TextScanner::GetParmIntFromLine(int lineNum, int index,std::string token /*= "|"*/)
{
	if (!(lineNum >= 0 && (uint32_t)lineNum < m_lines.size())) 
	{
		return 0;
	}
	if (token.size() != 1)
	{
		return 0;
	}

	return std::atoi(Utils::SeparateStringSTL(m_lines[lineNum], index, token[0]).c_str());
}

float TextScanner::GetParmFloatFromLine(int lineNum, int index,std::string token /*= "|"*/)
{
	if (!(lineNum >= 0 && (uint32_t)lineNum < m_lines.size())) 
	{
		return 0.f;
	}
	if (token.size() != 1)
	{
		return 0.f;
	}

	return (float)std::atof(Utils::SeparateStringSTL(m_lines[lineNum], index, token[0]).c_str());
}

void TextScanner::Replace(const std::string& thisStr, const std::string& thatStr)
{
	for (unsigned int i = 0; i < m_lines.size(); i++)
	{
		Utils::StringReplace(thisStr, thatStr, m_lines[i]);
	}
}

void TextScanner::DumpToLog()
{
	for (int i = 0; i < GetLineCount(); i++) 
	{
		std::string tmp = m_lines[i].c_str();
	    Utils::StringReplace("%", "%%", tmp);
	}
}

bool TextScanner::SaveFile(const std::string& fName /*= true*/)
{
	std::string f = fName;
	std::string lineFeed = "\r\n";
	FILE *fp = fopen(f.c_str(), "wb");

	if (!fp)
	{
		return false;
	}

	for (uint32_t i = 0; i < m_lines.size(); i++) 
	{
		fwrite(m_lines[i].c_str(), m_lines[i].size(), 1, fp);
		fwrite(lineFeed.c_str(), lineFeed.size(), 1, fp);
	}
	
	fclose(fp);
	return true;
}

void TextScanner::DeleteLine(int lineNum)
{
	if (m_lastLine && m_lastLine >= lineNum) 
	{
		m_lastLine--;
	}

	m_lines.erase(m_lines.begin()+lineNum);
}

std::vector<std::string> TextScanner::TokenizeLine(int lineNum, const std::string& theDelimiter /*= "|"*/)
{
	return Utils::StringTokenize(m_lines[lineNum], theDelimiter);
}

void TextScanner::AppendToFile(std::string fileName /*= true*/)
{
	if (m_lines.empty()) 
	{
		return;
	}

	FILE *fp = NULL;

#ifdef __linux__
    fp = fopen(fileName.c_str(), "a+");
#else
    fp = fopen(fileName.c_str(), "ab");
    if (!fp)
        fp = fopen(fileName.c_str(), "wb");
#endif

	if (!fp)
	{
		return;
	}

	std::string temp;
	for (uint32_t i = 0; i < m_lines.size(); i++) 
	{
		temp = m_lines[i]+"\r\n";
		fwrite(temp.c_str(), temp.size(), 1, fp);
	}

	fclose(fp);
}

bool TextScanner::AppendFromMemoryAddress(const char* pCharArray)
{
	std::vector<std::string> tempVec = Utils::StringTokenize(pCharArray, "\n");
	for (unsigned int i = 0; i < tempVec.size(); i++) 
	{
		Utils::StringReplace("\r", "", tempVec[i]);
		m_lines.push_back(tempVec[i]);
	}

	return true;
}

bool TextScanner::AppendFromString(const std::string lines)
{
	std::vector<std::string> tempVec= Utils::StringTokenize(lines, "\n");
	for (unsigned int i = 0; i < tempVec.size(); i++) 
	{
		Utils::StringReplace("\r", "", tempVec[i]);
		m_lines.push_back(tempVec[i]);
	}

	return true;
}

bool TextScanner::AppendFromMemoryAddressRaw(const char* pCharArray, int size)
{
	std::vector<std::string> tempVec= Utils::StringTokenize(pCharArray, "\n");
	for (unsigned int i = 0; i < tempVec.size(); i++)
	{
		m_lines.push_back(tempVec[i]);
	}
        
	return true;
}