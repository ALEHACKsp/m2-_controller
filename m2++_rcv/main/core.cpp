#include <inc.h>
#include <log.h>

namespace FS_packets//FromServer
{
	enum
	{
		OP_INVALID = 0,
		OP_EXCH,
		OP_PING,
		OP_MAX
	};
	struct s_header
	{
		short			opcode = OP_INVALID;
		short			size = 0;
	};

	struct s_exch
	{
		char			ipc_key[64 + 1] = "<empty>";
	};

}
namespace FC_packets
{
	enum
	{
		OP_INVALID = 0,
		OP_PING,
		OP_MAX
	};
	struct s_header
	{
		short	opcode = OP_INVALID;
		short	size = 0;
	};
	struct s_ping : s_header
	{
		char	message[64 + 1];
	};
}
int main()
{
	util::c_log::Instance().duo("[ starting rcv client ]\n");

	auto tcp_client = new CTCPClient([](std::string msg) { printf(msg.append("\n").c_str()); }, ASocket::NO_FLAGS);
	tcp_client->Connect("127.0.0.1", "2920");

	auto last_send = GetTickCount64();
	
	while (1)
	{
		if (GetTickCount64() > last_send)
		{
			last_send = GetTickCount64() + 2000;
			
			auto ping_pack = FC_packets::s_ping();
			ping_pack.size = sizeof(decltype(ping_pack));
			ping_pack.opcode = FC_packets::OP_PING;
			strcpy(ping_pack.message, "poggers from client!");

			char ping_dat[sizeof ping_pack];
			memcpy(ping_dat, &ping_pack, sizeof ping_pack);

			auto r = tcp_client->Send(ping_dat, sizeof ping_dat);
			if (r)
			{
				printf("[ sent ping to srv: %s ]\n", ping_pack.message);
			}
		}
		
		char peak_data[sizeof FS_packets::s_header] = {};
		auto h_byte = tcp_client->Receive(peak_data, sizeof peak_data, false);
		if (h_byte == sizeof FS_packets::s_header)
		{
			auto header = FS_packets::s_header();
			memcpy(&header, peak_data, sizeof FS_packets::s_header);

			if (header.opcode >= FS_packets::OP_MAX || header.opcode == FS_packets::OP_PING) continue;

			printf("[ header rcv: %i, size: %i, %i ]\n", header.opcode, header.size, h_byte);

			if (header.opcode == FS_packets::OP_EXCH)	
			{
				char pack_dat[65] = "";
				h_byte = tcp_client->Receive(pack_dat, sizeof FS_packets::s_exch);

				if (h_byte > 0)
				{
					auto exch = FS_packets::s_exch();
					memcpy(&exch, pack_dat, sizeof FS_packets::s_exch);
					//
					printf("[ recv s_exch(%i), ipc_key: %s]\n", sizeof exch, exch.ipc_key);
				}
			}			
		}
	}

	tcp_client->Disconnect();

	util::c_log::Instance().duo("[ disconnected ]\n");
	system("pause");

	while (true) {}
}