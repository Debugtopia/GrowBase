#include <BaseApp.h> // precompiled
#include <variant>
#include <stdarg.h>


#include <SDK/TimeWrapper.h>


BaseApp g_baseApp;
BaseApp __ptr GetBaseApp() { return &g_baseApp; }


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


// console log funcs
void LogMsg(const char* traceStr, ...)
{
	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset(buffer, 0, logSize);
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
	memset(buffer, 0, logSize);
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


Config GetConfig()
{
	return GetBaseApp()->GetConfig();
}

void BaseApp::Init()
{
	// setting up our server
	LogMsg("starting GrowBase");
	GetGrowConfig()->Load();
	GetItemInfoManager()->Load();
}