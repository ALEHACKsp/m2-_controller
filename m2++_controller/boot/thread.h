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
			s_thread_i(std::function<void(ULONGLONG p)> f, size_t d) : func(f), interval(d)
			{ }
			s_thread_i(std::function<void(ULONGLONG p)> f, size_t d, ULONGLONG uid) : func(f), interval(d), uid(uid)
			{ }
			s_thread_i(std::function<void(ULONGLONG p)> f, size_t d, ULONGLONG uid, ASocket::Socket sock) : func(f), interval(d), uid(uid), sock_to_client(sock)
			{ }
			s_thread_i(std::function<void(ULONGLONG p)> f, size_t d, ULONGLONG uid, ASocket::Socket sock, std::thread* t) : func(f), interval(d), uid(uid), sock_to_client(sock), thread(t)
			{ }
			std::function<void(ULONGLONG p)>func;
			size_t							interval = 0;
			ULONGLONG						last_exec = 0;
			//
			ULONGLONG						uid = 0;
			ASocket::Socket					sock_to_client = 0;
			//
			std::thread					    *thread;
		};
	}
	class c_thread : public s<c_thread>
	{
	private:
		std::function<void()>					core = {};
		std::vector<thread_strc::s_thread_i*>	pool = {};
		std::vector<thread_strc::s_thread_i*>	indep_pool = {};
	public:
		void		setup();
		void		work();
		bool		add(thread_strc::s_thread_i* t);
		bool		indep(thread_strc::s_thread_i* t);
		bool		destroy(ULONGLONG uid);
	};
}