#pragma once
#include <inc.h>
namespace net
{
	class c_packet : public s<c_packet>
	{
	private:
		
	public:
		template<class T>
		T		to_data(char* strc)
		{
			auto buf = T();
			memcpy(&buf, strc, sizeof T);
			return buf;
		}
		template<class T>
		[[nodiscard]] auto rcv_data(ASocket::Socket sock, bool full = true) -> std::pair<int, std::vector<char>>
		{
			auto dat = std::vector<char>(); dat.resize(sizeof T);
			auto recv_bytes = net::c_consock::Instance().server_instance->Receive(sock, &dat[0], sizeof T, full);
			return { recv_bytes, dat };
		}
		template<class T>
		[[nodiscard]] auto craft(void* strc) -> std::vector<char>
		{
			auto dat = std::vector<char>(); dat.reserve(sizeof T);
			memcpy(&dat[0], strc, sizeof T);
			return dat;
		}
		bool	send(char* dat, size_t size, ASocket::Socket sock);
		//FROM CLIENT handler
		bool	fc_handler(ULONGLONG uid, ASocket::Socket sock);
		bool	setup();
	};
}