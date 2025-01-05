#ifndef PROTONSDK__RTTEX__H
#define PROTONSDK__RTTEX__H
#include <cstdint>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

#include <zlib.h>


// global definitions
#define C_RTFILE_PACKAGE_LATEST_VERSION 0
#define C_RTFILE_PACKAGE_HEADER "RTPACK"
#define C_RTFILE_PACKAGE_HEADER_BYTE_SIZE 6


struct RTFileHeader
{
	char FileTypeID[C_RTFILE_PACKAGE_HEADER_BYTE_SIZE];
	uint8_t Version;
	uint8_t Reserved[1];
};

enum eCompressionType
{
	C_COMPRESSION_NONE = 0,
	C_COMPRESSION_ZLIB = 1
};

struct RTPackHeader
{
	RTFileHeader FileHeader;
	uint32_t CompressedSize;
	uint32_t DecompressedSize;
	uint8_t CompressionType;
	uint8_t Reserved[15];
};

#define RT_FORMAT_EMBEDDED_FILE 20000000
#define C_RTFILE_TEXTURE_HEADER "RTTXTR"
struct RTTexHeader
{
	RTFileHeader FileHeader;
	int32_t Height;
	int32_t Width;
	int32_t Format;
	int32_t OriginalHeight;
	int32_t OriginalWidth;
	unsigned char bUsesAlpha;
	unsigned char bAlreadyCompressed;
	unsigned char ReservedFlags[2];
	int32_t MipmapCount;
	int32_t Reserved[16];
};

struct RTTexMipHeader
{
	int32_t Height;
	int32_t Width;
	int32_t DataSize;
	int32_t MipLevel;
	int32_t Reserved[2];
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////


static std::vector<uint8_t> GetFileData(const std::string& fName) 
{
	std::ifstream in(fName, std::ios::binary | std::ios::ate);
	if (!in)
	{
		// failed to open
		return {};
	}

	// yuh grab de shit
	std::streamsize size = in.tellg();
	in.seekg(0, std::ios::beg);

	std::vector<uint8_t> buffer(size);
	if (!in.read(reinterpret_cast<char*>(buffer.data()), size)) 
	{
		// failed to read data
		return {};
	}

	return buffer;
}

static std::vector<uint8_t> GetMipData(const uint8_t * pData, size_t memOffset, int dataSize) 
{
	if (pData == NULL)
	{
		// data is null
		return {};
	}

	return std::vector<uint8_t>(pData + memOffset, pData + memOffset + dataSize);
}

static bool IsFileHeaderGood(const uint8_t * pData, size_t& memOffset, RTFileHeader& header)
{
	if (pData == NULL)
	{
		// data is null
		return false;
	}

	std::memcpy(&header, pData + memOffset, sizeof(RTFileHeader));
	memOffset += sizeof(RTFileHeader);
	if (std::strncmp(header.FileTypeID, C_RTFILE_PACKAGE_HEADER, C_RTFILE_PACKAGE_HEADER_BYTE_SIZE) != 0) 
	{
		// invalid compression type header
		return false;
	}

	return true;
}

static bool IsTexHeaderGood(const uint8_t * pData, size_t& memOffset, RTTexHeader& header)
{
	if (pData == NULL)
	{
		// data is null
		return false;
	}

	std::memcpy(&header, pData + memOffset, sizeof(RTTexHeader));
	memOffset += sizeof(RTTexHeader);
	if (std::strncmp(header.FileHeader.FileTypeID, C_RTFILE_TEXTURE_HEADER, C_RTFILE_PACKAGE_HEADER_BYTE_SIZE) != 0) 
	{
		// invalid compression type header
		return false;
	}

	return true;
}


// func to read and parse RTTexMipHeader
void ReadMipHeaders(const uint8_t * pData, size_t& memOffset, std::vector<RTTexMipHeader>& mipHeaders, int mipCount)
{
	if (pData == NULL)
	{
		// data is null
		return;
	}

	for (int i = 0; i < mipCount; ++i) 
	{
		RTTexMipHeader mipHeader;
		std::memcpy(&mipHeader, pData + memOffset, sizeof(RTTexMipHeader));
		memOffset += sizeof(RTTexMipHeader);

		mipHeaders.push_back(mipHeader);
	}
}

// proccess rttex file
void ProcessRTTexFile(const std::string& filePath) 
{
	try 
	{
		std::vector<uint8_t> data = GetFileData(filePath);
		size_t memOffset = 0;
		RTTexHeader header;

		if (!IsTexHeaderGood(data.data(), memOffset, header))
		{
			// wrong header
			return;
		}

		std::vector<RTTexMipHeader> mipHeaders;
		ReadMipHeaders(data.data(), memOffset, mipHeaders, header.MipmapCount);

		if (mipHeaders.empty())
		{
			// no headers found
			return;
		}

		const RTTexMipHeader& largestMip = mipHeaders[0];
		auto mipData = GetMipData(data.data(), memOffset, largestMip.DataSize);
		std::vector<uint8_t> pixelData;
		
		if (header.bAlreadyCompressed) 
		{
			return;
		}
		
		pixelData = mipData;
		if (pixelData.size() != largestMip.Width * largestMip.Height * 4) 
		{
			// some size issue
			return;
		}

		// save as png here XDXD
	}
	catch (const std::exception& ex) {
		printf("error procceding rt tex: %s", ex.what());
		return;
	}
}

#endif // PROTONSDK__RTTEX__H