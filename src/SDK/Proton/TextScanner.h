#pragma once
#include <vector>
#include <string>
#include <SDK/Proton/Math.h>

class TextScanner 
{
public:
	TextScanner();
	TextScanner(const char* pCharArray);
	TextScanner(const std::string& fName);

	~TextScanner();
	void Kill();

	bool LoadFile(const std::string& fName);
	bool SaveFile(const std::string& fName);
	std::string GetParmString(std::string label, int index, std::string token = "|");
	int GetParmInt(std::string label, int index, std::string token = "|");
	float GetParmFloat(std::string label, int index, std::string token = "|");
	std::string GetParmStringFromLine(int lineNum, int index, std::string token = "|");
	int GetParmIntFromLine(int lineNum, int index, std::string token = "|");
	float GetParmFloatFromLine(int lineNum, int index, std::string token = "|");
	std::string GetMultipleLineStrings(std::string label, std::string token = "|");
	std::string GetLine(int lineNum); //0 based, returns "" if out of range
	void Replace(const std::string& thisStr, const std::string& thatStr);
	bool IsLoaded() { return !m_lines.empty(); }
	bool SetupFromMemoryAddress(const char* pCharArray);
	bool SetupFromMemoryAddressRaw(const char* pCharArray, int size);
	void DeleteLine(int lineNum);
	std::string GetAllRaw();
	std::vector<std::string> GetLines() const { return m_lines; }
	int GetLineCount() { return (int)m_lines.size(); }
	void DumpToLog();
	std::vector<std::string> TokenizeLine(int lineNum, const std::string& theDelimiter = "|");
	void AppendToFile(std::string fileName);
	bool AppendFromMemoryAddress(const char* pCharArray);
	bool AppendFromMemoryAddressRaw(const char* pCharArray, int size);
	bool AppendFromString(const std::string lines);

public:
    bool Contain(const std::string& key, int index = 1, const std::string& token = "|", int key_index = 0) 
	{
        return GetParmString(key, index, token) != "" ? true : false;
    }
    
private:
	int m_lastLine;
	std::vector<std::string> m_lines;

};