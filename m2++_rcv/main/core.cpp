#include <inc.h>
#include <log.h>
namespace FS_packets//FromServer
{
	enum
	{
		OP_INVALID = 0,
		OP_EXCH,
		OP_IDENT,
		OP_PING,
		OP_MAX
	};
	struct s_header
	{
		int				opcode = OP_INVALID;
		int				size = 0;
	};

	struct s_exch
	{
		char			ipc_key[64+1] = "<empty>";
	};

	struct s_ping
	{
		char			ping[4+1] = "<>";
	};

	struct s_ident
	{
		ULONGLONG		ident = 0;
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
		int			opcode = OP_INVALID;
		int			size = 0;
	};
	struct s_ping : s_header
	{
		char		message[64+1];
	};
}
#define MAX_PACK 512
#define vartostr(var) #var
ULONGLONG identity = 0;
std::string ipc_key = "";
void rcv_thread(CTCPClient* client)
{
	while (true)
	{
		if (!client->IsConnected()) break;

		char buf[MAX_PACK] = {};
		auto recv_count = client->Receive(buf, sizeof buf, false);//read until timeout

		if (recv_count > 0)
		{
			auto as_header = FS_packets::s_header();
			memcpy(&as_header, buf, sizeof as_header);
			//
			printf("[ %s: %i, %s: %i ]\n", vartostr(as_header.opcode), as_header.opcode, vartostr(as_header.size), as_header.size);
			//
			switch (as_header.opcode)
			{
			case FS_packets::OP_EXCH:
			{
				auto pack_exch = FS_packets::s_exch();
				memcpy(&pack_exch, buf + sizeof as_header, sizeof pack_exch);
				//
				printf("[ ( %s --> ) %s: %s ]\n", vartostr(FS_packets::OP_EXCH), vartostr(pack_exch.ipc_key), pack_exch.ipc_key);
				//
				ipc_key = pack_exch.ipc_key;
				break;
			}
			case FS_packets::OP_IDENT:
			{
				auto pack_ident = FS_packets::s_ident();
				memcpy(&pack_ident, buf + sizeof as_header, sizeof pack_ident);
				//
				printf("[ ( %s --> ) %s: %llu ]\n", vartostr(FS_packets::OP_IDENT), vartostr(pack_ident.ident), pack_ident.ident);
				//
				identity = pack_ident.ident;
				break;
			}
			case FS_packets::OP_PING:
			{
				auto pack_ping = FS_packets::s_ping();
				memcpy(&pack_ping, buf + sizeof as_header, sizeof pack_ping);
				//
				//printf("[ ( %s --> ) %s: %s ]\n", vartostr(FS_packets::OP_PING), vartostr(pack_ping.ping), pack_ping.ping);
				break;
			}
			default: break;
			}
		}
	}
}
int main()
{
	util::c_log::Instance().duo("[ starting rcv client ]\n");

	auto tcp_client = new CTCPClient([](std::string msg) { printf(msg.append("\n").c_str()); }, ASocket::NO_FLAGS);
	tcp_client->Connect("127.0.0.1", "669");

	std::thread rcv(rcv_thread, tcp_client);
	rcv.detach();

	auto last_send = GetTickCount64();
	while (1)
	{
		if (GetTickCount64() > last_send)
		{
			last_send = GetTickCount64() + 2000;

			auto ping_pack = FC_packets::s_ping();
			ping_pack.size = sizeof(decltype(ping_pack));
			ping_pack.opcode = FC_packets::OP_PING;
			strcpy(ping_pack.message, std::string("hi from ").append(std::to_string(identity)).c_str());

			char ping_dat[sizeof ping_pack];
			memcpy(ping_dat, &ping_pack, sizeof ping_pack);

			auto r = tcp_client->Send(ping_dat, sizeof ping_dat);
			if (r)
			{
				printf("[ ( %s <-- ) %s: %s ]\n", vartostr(FC_packets::OP_PING), vartostr(ping_pack.message), ping_pack.message);
			}
		}


	}

	while (tcp_client->IsConnected()) { std::this_thread::sleep_for(5s); };

	tcp_client->Disconnect();

	util::c_log::Instance().duo("[ disconnected ]\n");
	system("pause");

	while (true) {}
}