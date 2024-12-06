#include <BaseApp.h> // precompiled
#include <SDK/Proton/FileSystem/StreamingInstanceFile.h>
#pragma warning(disable : 4996)

StreamingInstanceFile::StreamingInstanceFile() : m_fp(NULL) 
{
    //
}

StreamingInstanceFile::~StreamingInstanceFile() 
{
	Close();
}

void StreamingInstanceFile::Close() 
{
	if (!m_fp)
	{
		return;
	}

    fclose(m_fp);
    m_fp = NULL;
}

bool StreamingInstanceFile::Open(std::string fName) 
{
	Close();
	m_fp = fopen(fName.c_str(), "rb");

	return m_fp != NULL;
}

int StreamingInstanceFile::Read(uint8_t* pBufferOut, int maxBytesToRead) 
{
	if (!m_fp)
	{
		return 0;
	}

	int bytesRead = (int)fread(pBufferOut, 1, maxBytesToRead, m_fp);
	return bytesRead;
}

bool StreamingInstanceFile::IsFinished() 
{
	if (!m_fp) 
	{
		return true;
	}

	return feof(m_fp) != 0;
}

void StreamingInstanceFile::SeekFromStart(int byteCount) 
{
	if (!m_fp) 
	{
		return;
	}

	fseek(m_fp, byteCount, SEEK_SET);
}