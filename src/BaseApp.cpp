#include <BaseApp.h> // precompiled
#include <variant>
#include <stdarg.h>

#include <Server/ENetServer.h>

#include <Client/GameClient.h>

#include <SDK/TimeWrapper.h>


BaseApp g_baseApp;
BaseApp * GetBaseApp() { return &g_baseApp; }


BaseApp::BaseApp()
{
	g_logFile.open("log.txt", std::ios::out | std::ios::app);
}

BaseApp::~BaseApp()
{
	if (g_logFile.is_open())
	{
		g_logFile.close();
	}
}

// memory funcs
void nova_memcopy(void* pSourceTo, const void* pSource, size_t sourceSize, int& offset)
{
	if (pSourceTo == NULL)
	{
		// won't copy for safety
		return;
	}

	memcpy(pSourceTo, pSource, sourceSize);
	offset += (int)sourceSize;
}

void nova_strcopy(void* pSourceTo, const std::string& str, size_t sourceSize, int& offset)
{
	if (pSourceTo == NULL)
	{
		// won't copy for safety
		return;
	}

	u16 strLen = (u16)str.length();
	memcpy(pSourceTo, &strLen, 2);
	offset += 2;
	memcpy(static_cast<char*>(pSourceTo) + 2, str.c_str(), strLen);
	offset += (int)strLen;
}

void nova_dealloc(void* pSource)
{
	if (pSource == NULL)
	{
		// there is nothing to de-allocate
		return;
	}

	free(pSource);
}

void nova_delete(void* pSource)
{
	if (pSource == NULL)
	{
		// there is nothing to de-allocate
		return;
	}

	delete pSource;
}

void nova_delete_arr(void* pSource)
{
	if (pSource == NULL)
	{
		// there is nothing to de-allocate
		return;
	}

	delete[] pSource;
}

// console log funcs
void LogMsg(const char* traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	std::memset(buffer, 0, logSize);
	va_start(argsVA, traceStr);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, traceStr, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, traceStr, argsVA);
#else
	vsnprintf(buffer, logSize, traceStr, argsVA);
#endif

	va_end(argsVA);
	printf("%s\n", buffer);

	if (g_logFile.is_open())
	{
		// logging into file
		g_logFile << "S" + std::to_string(GetBaseApp()->GetServerID()) + ":" << GetTimeAsString() << "> " << std::string(buffer) << std::endl;
		g_logFile.flush();
	}
}

void LogError(const char* traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	std::memset(buffer, 0, logSize);
	va_start(argsVA, traceStr);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, traceStr, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, traceStr, argsVA);
#else
	vsnprintf(buffer, logSize, traceStr, argsVA);
#endif

	va_end(argsVA);
	printf("%s\n", buffer);

	if (g_logFile.is_open())
	{
		// logging into file
		g_logFile << "S" + std::to_string(GetBaseApp()->GetServerID()) + ":" << GetTimeAsString() << " ERROR > " << std::string(buffer) << std::endl;
		g_logFile.flush();
	}
}

// log funcs
void LogMsgSafe(const char* traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	std::memset(buffer, 0, logSize);
	va_start(argsVA, traceStr);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, traceStr, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, traceStr, argsVA);
#else
	vsnprintf(buffer, logSize, traceStr, argsVA);
#endif

	va_end(argsVA);
	if (g_logFile.is_open())
	{
		// logging into file
		g_logFile << "S" + std::to_string(GetBaseApp()->GetServerID()) + ":" << GetTimeAsString() << "> " << std::string(buffer) << std::endl;
		g_logFile.flush();
	}
}

void LogErrorSafe(const char* traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	std::memset(buffer, 0, logSize);
	va_start(argsVA, traceStr);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, traceStr, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, traceStr, argsVA);
#else
	vsnprintf(buffer, logSize, traceStr, argsVA);
#endif

	va_end(argsVA);
	if (g_logFile.is_open())
	{
		// logging into file
		g_logFile << "S" + std::to_string(GetBaseApp()->GetServerID()) + ":" << GetTimeAsString() << " ERROR > " << std::string(buffer) << std::endl;
		g_logFile.flush();
	}
}


Config GetConfig()
{
	return GetBaseApp()->GetConfig();
}

bool IsBeta()
{
	return GetConfig().bBetaServer;
}

void BaseApp::Init()
{
	// setting up our server
	LogMsg("starting GrowBase");
	GetGrowConfig()->Load(m_config);

	//GetItemInfoManager()->LoadFile();
	GetItemInfoManager()->Load();
	GetItemInfoManager()->Serialize(5);

	GetENetServer()->Run(GetConfig().address.c_str(), GetConfig().basePort);
}