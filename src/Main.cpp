#include <iostream>
#include <chrono>
#include <thread>

#include <BaseApp.h>

int main()
{
	GetBaseApp()->Init();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}