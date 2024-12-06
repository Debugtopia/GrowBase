#include <BaseApp.h> // precompiled
#include <SDK/Proton/FileSystem/StreamingInstance.h>

StreamingInstance::StreamingInstance()
{
	//
}

StreamingInstance::~StreamingInstance()
{
	//
}

bool StreamingInstance::ReadLineOfText(std::string& textOut)
{
	//OPTIMIZE: Oh come on, there's got to be a better way
	textOut.clear();

	if (IsFinished()) 
	{
		return false;
	}

	char buffer[4048];
	int cur = 0;

	while(1)
	{
		if (!Read((uint8_t*)&buffer[cur], 1))
		{
			//eof I guess
			buffer[cur] =0;
			textOut = buffer;
			if (cur == 0) return false;
			return true;
		}
		if (buffer[cur] == '\r')
		{
			continue; //skip those dumb things
		}
		if (buffer[cur] == '\n')
		{
			buffer[cur] = 0;
			textOut = buffer;
			return true;
		}
		
		cur++;
		//textOut += c;
	}

}


bool StreamingInstance::ReadLineOfText(char* buffer, int bufferSize)
{
	//OPTIMIZE: Oh come on, there's got to be a better way

	if (IsFinished())
	{
		return false;
	}

	int cur = 0;

	while(1)
	{
		if (!Read((uint8_t*)&buffer[cur], 1))
		{
			//eof I guess
			buffer[cur] =0;
			
			if (cur == 0) 
			{
				return false;
			}

			return true;
		}

		if (buffer[cur] == '\r')
		{
			continue; //skip those dumb things
		}

		if (buffer[cur] == '\n')
		{
			buffer[cur] = 0;
			return true;
		}

		cur++;
		if (bufferSize <= cur)
		{
			buffer[cur] = 0;
			return true;
		}

		//textOut += c;
	}

}

void StreamingInstance::SeekFromStart(int byteCount)
{
}

int32_t StreamingInstance::ReadInt32()
{
	int tmp;
	Read((uint8_t*)&tmp, sizeof(int32_t));
	return tmp;
}

float StreamingInstance::ReadFloat32()
{
	float tmp;
	Read((uint8_t*)&tmp, sizeof(int32_t));
	return tmp;
}