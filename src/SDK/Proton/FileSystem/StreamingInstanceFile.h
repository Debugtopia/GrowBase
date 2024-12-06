#include <SDK/Proton/FileSystem/StreamingInstance.h>

class StreamingInstanceFile: public StreamingInstance
{
public:
	StreamingInstanceFile();
	virtual ~StreamingInstanceFile();

	virtual bool Open(std::string fName);
	virtual bool IsFinished();
	virtual int Read(uint8_t* pBufferOut, int maxBytesToRead);
	virtual void Close();
	virtual void SeekFromStart(int byteCount);
protected:
	

private:
	FILE* m_fp;
};