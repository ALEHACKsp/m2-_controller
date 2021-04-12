#pragma once
#include <inc.h>
namespace util
{
	class c_log : public s<c_log>
	{
	private:
		const char*		log_file = "M2++CONTROLLER_LOG.TXT";
		std::ofstream	fstream;
		void			to_console(const char* in);
		void			to_file(const char* in);
	public:
		void			setup();
		const char*		string(const char* in, ...);
		void			print(const char* in, ...);
		void			log(const char* in, ...);
		void			duo(const char* in, ...);
	};
}