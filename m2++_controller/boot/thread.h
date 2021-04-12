#pragma once
#include <inc.h>
namespace boot
{
	namespace thread_ext
	{
		extern void __stdcall setup();
	}
	namespace thread_strc
	{
		struct s_thread_i
		{
			s_thread_i(std::function<void()> &f, size_t &d) : func(f), interval(d)
			{ }
			std::function<void()>	func;
			size_t					interval = 0;
			ULONGLONG				last_exec = 0;
		};
	}
	class c_thread : public s<c_thread>
	{
	private:
		std::function<void()>					core = {};
		std::vector<thread_strc::s_thread_i*>	pool = {};
	public:
		void		setup();
		void		work();
		bool		add(thread_strc::s_thread_i* t);
	};
}