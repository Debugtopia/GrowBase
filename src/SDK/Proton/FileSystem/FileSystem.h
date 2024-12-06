#pragma once
#include <cstdint>
#include <string>

class StreamingInstance;
class FileSystem
{
public:
	FileSystem();
	virtual ~FileSystem();

	virtual uint8_t* Get( std::string fileName, int* pSizeOut) = 0;
	virtual StreamingInstance* GetStreaming(std::string fileName, int* pSizeOut) = 0; //pSizeOut currently always set to 0.  Returns null on fail. You must DELETE !
	virtual bool FileExists(std::string fileName) = 0;
	virtual int GetFileSize(std::string fileName) = 0;
};