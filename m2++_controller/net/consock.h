#pragma once
#include <inc.h>
namespace net
{
	namespace consock_strc
	{
		struct s_connection
		{
			//default gen new identity
			s_connection()
			{
				this->identity = randull(ULLONG_MAX);				
			}			
			ULONGLONG		identity = 0;		//this will be sort of a auth key, so we can identify each client
			ULONGLONG		last_packet = 0;	//last time packet was rcv/sent
			ULONGLONG		packets_rcv = 0;	//total rcv packs
			ULONGLONG		packets_sent = 0;	//total packets sent to bot
		};
	}
	class c_consock : public s<c_consock>
	{
	private:
		SOCKET										ipc_sock = {};
		std::vector<consock_strc::s_connection*>	connections = {};
		bool	init_port();
	public:
		bool	setup();
		void	connector();
	};
}