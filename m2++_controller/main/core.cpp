#include <inc.h>
int main()
{
	boot::c_thread::Instance().setup();

	while (true) { std::this_thread::sleep_for(1s); }

	//ui
}

