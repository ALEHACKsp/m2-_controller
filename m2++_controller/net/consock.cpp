#include "consock.h"

bool net::c_consock::init_port()
{
	uvirt;	
	SOCKADDR_IN clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(XorStr("127.0.0.1"));
	clientService.sin_port = htons(std::stoi(boot::c_conf::Instance().base_config.ipc_port));

	this->ipc_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->ipc_sock == INVALID_SOCKET)
	{
		closesocket(this->ipc_sock);
		dbglog(XorStr("[ opening socket [%s] failed => %ld ]\n"), boot::c_conf::Instance().base_config.ipc_port.c_str(), WSAGetLastError());
		return false;
	}
	if (connect(this->ipc_sock, (SOCKADDR*)&clientService, sizeof(clientService)) != SOCKET_ERROR)
	{
		closesocket(this->ipc_sock);
		dbglog(XorStr("[ socket [%s] already used => %ld ]\n"), boot::c_conf::Instance().base_config.ipc_port.c_str(), WSAGetLastError());
		return false;
	}
	if (listen(this->ipc_sock, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(this->ipc_sock);
		dbglog(XorStr("[ failed listen for socket [%s] ]\n"), boot::c_conf::Instance().base_config.ipc_port.c_str());
		return false;
	}	
	vmend;
	return true;
}

bool net::c_consock::setup()
{
	uvirt;
	if (!this->init_port())
	{
		dbglog(XorStr("[ ipc port init failed ]\n"));
		return false;
	}
	boot::c_thread::Instance().add(new boot::thread_strc::s_thread_i(([this](void* data)
		{
			net::c_consock::Instance().connector();//to execute the connection acceptor
		}), 0, 0));
	dbglog(XorStr("[ ipc port init completed, routine regsitered ]\n"));
	vmend;
	return true;
}

void net::c_consock::connector()
{
	uvirt;
	SOCKET client_socket;
	if ((client_socket = accept(this->ipc_sock, NULL, NULL))) 
	{
		auto construct = new net::consock_strc::s_connection();
		//check for unique
		while (true)
		{
			auto matched = false;//will re-run if once matched to ensure new generated identity isnt listed either
			for (auto&& obj : net::c_consock::Instance().connections)
			{
				if (construct->identity != obj->identity) continue;
				delete construct;//del and re init
				construct = new net::consock_strc::s_connection();
				matched = true;
			}
			if (!matched) break;
		}		
		dbglog(XorStr("[ new bot joined, identity: %ull ]\n"), construct->identity);
		boot::c_thread::Instance().add(new boot::thread_strc::s_thread_i(([this](void* data)
		{
				auto const* identity = (int*)data;
			
				dbglog(XorStr("[ thread tick from identity: %ull ]\n"), identity);
		}), 15, construct->identity, client_socket));
	}
	vmend;
}
