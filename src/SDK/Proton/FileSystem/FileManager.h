#pragma once
#include <list>
#include <SDK/Proton/FileSystem/FileSystem.h>

class FileSystem;
class StreamingInstance;
class FileInstance
{
public:
	FileInstance(const std::string& fileName);
	FileInstance() { m_size = 0; m_pData = NULL;}

	~FileInstance();

	bool IsLoaded() {return m_pData != NULL;}
	uint8_t* GetAsBytes() {return m_pData;}
	char* GetAsChars() {return (char*)m_pData;}
	int GetSize() {return m_size;}
	bool Load(std::string fileName);

private:
	
	void Kill();

	uint8_t *m_pData;
	int m_size;

};

class FileManager
{
public:
	FileManager();
	virtual ~FileManager();

	/**
	 * Loads a file from the file system and returns the contents of it as an array of bytes.
	 *
	 * If the file is compressed (an RTPack file), this also decompresses the file.
	 * The size of the (decompressed) data is written to \a pSizeOut.
	 *
	 * Returns \c NULL if anything goes wrong in the reading.
	 *
	 * A zero byte is added to the end of the file contents. This makes it easier to use
	 * the contents of the file as a string.
	 *
	 * The caller is responsible for freeing the returned memory later.
	 */

	uint8_t* Get(std::string fileName, int* pSizeOut, bool bAutoDecompress = true); //you must SAFE_DELETE_ARRAY() what it returns...
	StreamingInstance* GetStreaming(std::string fileName, int* pSizeOut); //pSizeOut currently always set to 0.  Returns null on fail. You must DELETE !
	void MountFileSystem(FileSystem* pFileSystem);
	bool FileExists(std::string fileName);
	int GetFileSize(std::string fileName);
	FileSystem* GetFileSystem(int index); //returns null if invalid index
	bool Copy(std::string srcFile, std::string dstFile); //copy a file to a file.  Destination can't be inside a zip, but src can be.

private:
	std::list<FileSystem*> m_fileSystems;

};

FileManager* GetFileManager();