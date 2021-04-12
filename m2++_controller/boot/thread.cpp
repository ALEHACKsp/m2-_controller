#include "thread.h"

void __stdcall boot::thread_ext::setup()
{
	uvirt;
	util::c_log::Instance().setup();
	boot::c_conf::Instance().setup();
	
	
	util::c_log::Instance().duo(XorStr("[ completed boot setup ]\n"));
	vmend;
}

void boot::c_thread::setup()
{
	uvirt;
	this->core = ([this]
	{
		boot::thread_ext::setup();
	});
	std::thread(this->core).join();
	vmend;
}

void boot::c_thread::work()
{
	while (true)
	{
		uvirt;
		auto time = GetTickCount64();
		for (auto&& obj : this->pool)
		{
			if (obj->last_exec + obj->interval < time) continue;
			obj->last_exec = time + obj->interval;
			obj->func();
		}
		std::this_thread::sleep_for(10ms);
		vmend;
	}
}

bool boot::c_thread::add(thread_strc::s_thread_i* t)
{
	uvirt;
	this->pool.push_back(t);
	vmend;
	return 1;	
}
