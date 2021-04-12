#include <inc.h>
int main()
{
	boot::c_thread::Instance().setup();
	//escape main
	std::thread mwork([]
	{
		boot::c_thread::Instance().work();
	});
	mwork.join();
}

