#pragma once
#include <string>

class StreamingInstance 
{
public:
	StreamingInstance();
	virtual ~StreamingInstance();

	virtual bool Open(std::string fName) = 0;
	virtual bool ReadLineOfText(std::string &textOut);
	virtual bool ReadLineOfText(char *pBuffer, int bufferSize);
	virtual bool IsFinished() = 0;
	virtual int Read(uint8_t * pBufferOut, int maxBytesToRead) = 0;
	virtual void SeekFromStart(int byteCount);
	
	int32_t ReadInt32();
	float ReadFloat32();
};