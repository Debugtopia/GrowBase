#include <BaseApp.h> // precompiled
#include <SDK/Proton/FileSystem/FileManager.h>
#include <SDK/Proton/MiscUtils.h>
#include <SDK/Proton/FileSystem/FileSystem.h>
#include <SDK/Proton/FileSystem/StreamingInstanceFile.h>
#pragma warning(disable : 4996)


FileManager g_fileManager;
FileManager* GetFileManager() { return &g_fileManager; }

FileInstance::FileInstance(const std::string& fileName) : m_pData(NULL) 
{
	Load(fileName);
}

void FileInstance::Kill() 
{
	if (m_pData)
	{
		delete m_pData;
	}

	m_size = 0;
}

FileInstance::~FileInstance() 
{
	Kill();
}

bool FileInstance::Load(std::string fileName) 
{
	Kill();
	m_pData = GetFileManager()->Get(fileName, &m_size);

	return IsLoaded();
}

FileManager::FileManager() 
{
    //
}

FileManager::~FileManager() 
{
	std::list<FileSystem*>::iterator itor = m_fileSystems.begin();
	while (!m_fileSystems.empty()) 
	{
		FileSystem* pFileSystem = *m_fileSystems.begin();
		m_fileSystems.pop_front();
		delete pFileSystem;
	}
}

StreamingInstance* FileManager::GetStreaming(std::string fileName, int* pSizeOut) 
{
	StreamingInstance* pStreaming = NULL;
	std::list<FileSystem*>::reverse_iterator itor = m_fileSystems.rbegin();
	while (itor != m_fileSystems.rend()) 
	{
		pStreaming = (*itor)->GetStreaming(fileName, pSizeOut);
		if (pStreaming) 
		{
			return pStreaming;
		}

        itor++;
	}

	StreamingInstanceFile* pStreamingFile = new StreamingInstanceFile();
	if (!pStreamingFile->Open(fileName)) 
	{ 
		delete pStreamingFile;
		return NULL;
	}

	return pStreamingFile;
}

uint8_t* FileManager::Get(std::string fileName, int* pSizeOut, bool bAutoDecompress) 
{	
    uint8_t* pData = NULL;
    std::list<FileSystem*>::reverse_iterator itor = m_fileSystems.rbegin();
    while (itor != m_fileSystems.rend()) 
	{
        pData = (*itor)->Get(fileName, pSizeOut);
        if (pData) 
		{
			break;
		}

        itor++;
    }

    if (!pData)
    {
        FILE *fp = fopen(fileName.c_str(), "rb");
        if (!fp)
		{
			return NULL;
		}

        fseek(fp, 0, SEEK_END);
        *pSizeOut = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        pData = (uint8_t*)new uint8_t[((*pSizeOut) +1)];
        if (!pData)
		{
			return 0;
		}
        
        pData[*pSizeOut] = 0; 
        size_t temp = fread(pData, *pSizeOut, 1, fp);
        fclose(fp);
    }

    /*
    if (bAutoDecompress && IsAPackedFile(pData)) 
	{
        unsigned int decompressedSize;
        uint8_t* pDecompressedData = DecompressRTPackToMemory(pData, &decompressedSize);
        *pSizeOut = decompressedSize;
        SAFE_DELETE_ARRAY(pData);
        return pDecompressedData;
    }*/

    return pData;
}

void FileManager::MountFileSystem(FileSystem* pFileSystem) 
{
	m_fileSystems.push_back(pFileSystem);
}

bool FileManager::FileExists(std::string fileName)
{
    std::list<FileSystem*>::reverse_iterator itor = m_fileSystems.rbegin();
	while (itor != m_fileSystems.rend()) 
	{
		if ((*itor)->FileExists(fileName)) 
		{
			return true;
		}

		itor++;
    }

	FILE *fp = fopen((fileName).c_str(), "rb");
	if (!fp)	
	{
		return false;
	}

	fclose(fp);
	return true;
}

int FileManager::GetFileSize(std::string fileName) 
{
	std::list<FileSystem*>::reverse_iterator itor = m_fileSystems.rbegin();
	while (itor != m_fileSystems.rend())
	{
		int fileSize = ((*itor)->GetFileSize(fileName));
		if (fileSize >= 0) 
		{
			return fileSize;
		}

		itor++;
	}

	return GetFileSize(fileName);
}

FileSystem* FileManager::GetFileSystem(int index) 
{
	if (index < 0 || index > (int)m_fileSystems.size())
	{
		return 0;
	}

	std::list<FileSystem*>::iterator itor = m_fileSystems.begin();
	int count = 0;
	while (itor != m_fileSystems.end()) 
	{
		if (count++ == index) 
		{
			return (*itor);
		}

		itor++;
	}

	return NULL;
}

bool FileManager::Copy(std::string srcFile, std::string dstFile) 
{
	int size;
	StreamingInstance* pSrc = GetFileManager()->GetStreaming(srcFile, &size);
	if (!pSrc)
	{
		return false;
	}

	const int bufferSize = 512;
	uint8_t buff[bufferSize];

	FILE *fp = fopen(dstFile.c_str(), "wb");
	if (!fp) 
	{
		delete pSrc;
		return false;
	}

	while (!pSrc->IsFinished()) 
	{
		int bytesRead = pSrc->Read(buff, bufferSize);
		if (bytesRead > 0)
		{
			fwrite(buff, bytesRead, 1, fp);
		}
	}

	fclose(fp);
	delete pSrc;
	return true;
}
