#include "packet_handler.h"

bool net::c_packet::send(char* dat, size_t size, ASocket::Socket sock)
{
	return net::c_consock::Instance().server_instance->Send(sock, dat, size);
}

bool net::c_packet::fc_handler(ULONGLONG uid, ASocket::Socket sock)
{
	uvirt;
	auto recv_dat = this->rcv_data<FC_packets::s_header>(sock, false);
	if (recv_dat.first > 0)
	{
		const auto header = this->to_data<FC_packets::s_header>(recv_dat.second.data());
		if (header.opcode > FC_packets::OP_MAX) return false;
		if (header.opcode == FC_packets::OP_PING)
		{
			dbglog(XorStr("[ ( --> ) header: %s, size: %i ]\n"), vartostr(FC_packets::OP_PING), header.size);
			auto ping_dat = this->rcv_data<FC_packets::s_ping>(sock);
			if (ping_dat.first > 0)
			{
				const auto ping_packet = this->to_data<FC_packets::s_ping>(ping_dat.second.data());
				dbglog(XorStr("[ ( --> ) message: %s, %i ]\n"), ping_packet.message, ping_dat.first );
			}
		}
	}
	vmend;
	return true;
}

bool net::c_packet::setup()
{

	return true;
}
