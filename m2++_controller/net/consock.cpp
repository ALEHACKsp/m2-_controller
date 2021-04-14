#include "consock.h"

#include "packet_handler.h"

bool net::c_consock::init_port()
{
	this->server_instance = new CTCPServer(
				[](const std::string msg) { dbglog(std::string(msg).append("\n").c_str()); }, 
				boot::c_conf::Instance().base_config.ipc_port, ASocket::NO_FLAGS);
	
	return true;
}

net::consock_strc::s_connection* net::c_consock::get(ULONGLONG uid)
{
	for (auto&& a : this->connections) if (a->identity == uid) return a;
	return nullptr;
}

bool net::c_consock::setup()
{
	uvirt;
	if (!this->init_port())
	{
		dbglog(XorStr("[ ipc port init failed ]\n"));
		return false;
	}
	auto t = std::thread([]()
	{
		uvirt;
		while (true) net::c_consock::Instance().connector();
		vmend;
	});
	this->connector_thread = &t;
	this->connector_thread->detach();
	dbglog(XorStr("[ ipc port init completed, routine regsitered ]\n"));
	vmend;
	return true;
}

void net::c_consock::connector()
{
	uvirt;

	dbglog(XorStr("[ waiting for connections ... ]\n"));

	ASocket::Socket client_socket = 0;
	if (this->server_instance->Listen(client_socket) && client_socket != INVALID_SOCKET)
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
		construct->socket = client_socket;
		net::c_consock::Instance().connections.push_back(construct);
		
		dbglog(XorStr("[ new bot joined, identity: %llu, %04x ]\n"), construct->identity, (uint32_t)client_socket);

		auto exch_packet = FS_packets::s_exch(boot::c_conf::Instance().base_config.ipc_comkey.c_str());
		auto as_dat = net::c_packet::Instance().craft<FS_packets::s_exch>((void*)&exch_packet);
		const auto exch_res = this->server_instance->Send(client_socket, &as_dat[0], exch_packet.size);

		auto ident_packet = FS_packets::s_ident(construct->identity);
		auto ident_dat = net::c_packet::Instance().craft<FS_packets::s_ident>((void*)&ident_packet);
		const auto ident_res = this->server_instance->Send(client_socket, &ident_dat[0], ident_packet.size);
		
		boot::c_thread::Instance().indep(new boot::thread_strc::s_thread_i(([this, client_socket](ULONGLONG data)
		{
				const auto client_identity = data;

				auto ping_packet = FS_packets::s_ping();
				auto as_dat = net::c_packet::Instance().craft<FS_packets::s_ping>((void*)&ping_packet);

				const auto ping_ret = this->server_instance->Send(client_socket, &as_dat[0], ping_packet.size);
				if (!ping_ret)
				{
					this->destroy(client_identity);
					ExitThread(0);
					return;
				}

				net::c_packet::Instance().fc_handler(client_identity, client_socket);

		}), 50, construct->identity, client_socket));
	}
	vmend;
}
bool net::c_consock::destroy(ULONGLONG uid)
{
	uvirt;

	for (size_t i = 0; i < this->connections.size(); i++)
	{
		auto obj = this->connections[i];
		if (!obj || obj->identity != uid) continue;
		this->connections.erase(this->connections.begin() + i);
		delete obj;
		dbglog(XorStr("[ dropped connection of client %llu ]\n"), uid);
		break;
	}

	boot::c_thread::Instance().destroy(uid);
	
	vmend;	
	return true;
}