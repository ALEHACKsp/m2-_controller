#include <inc.h>
int main()
{
#if _USE_CONSOLE == 0
	auto console_handle = GetConsoleWindow();
	FreeConsole();
	PostMessage(console_handle, WM_CLOSE, 0, 0);
#endif
	
	boot::c_thread::Instance().setup();
	std::thread mwork([]
	{
		boot::c_thread::Instance().work();
	});
	mwork.join();
}

