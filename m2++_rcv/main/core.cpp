#include <inc.h>
#include <log.h>

namespace FS_packets//FromServer
{
	enum
	{
		OP_INVALID = 0,
		OP_EXCH
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

int main()
{
	util::c_log::Instance().duo("[ starting rcv client ]\n");

	auto tcp_client = new CTCPClient([](std::string msg) { printf(msg.append("\n").c_str()); }, ASocket::ALL_FLAGS);
	tcp_client->Connect("127.0.0.1", "5573");

	while (1)
	{
		/*char data[256] = "";
		strcpy(data, "test message from CLIENT");

		auto r = tcp_client->Send(data, 256);
		if (r) printf("[ snd: %s ]\n", data);*/

		char peak_data[sizeof FS_packets::s_header] = "";
		auto h_byte = tcp_client->Receive(peak_data, sizeof FS_packets::s_header);
		if (h_byte > 0)
		{
			auto header = FS_packets::s_header();
			memcpy(&header, peak_data, sizeof FS_packets::s_header);

			printf("[ header rcv: %i, size: %i ]\n", header.opcode, header.size);

			if (header.opcode == FS_packets::OP_EXCH)
			{
				char pack_dat[sizeof FS_packets::s_exch];
				h_byte = tcp_client->Receive(pack_dat, sizeof FS_packets::s_exch);

				if (h_byte > 0)
				{
					auto exch = FS_packets::s_exch();
					memcpy(&exch, pack_dat, sizeof FS_packets::s_exch);
					//
					printf("[ recv s_exch(%i), IPC: %s ]\n", sizeof exch, exch.ipc_key);
				}
			}
		}

		//auto d = tcp_client->Receive(data, 256);
		//if (d > 0)
		//{
		//	if (d == sizeof FS_packets::s_exch)
		//	{
		//		auto exch = FS_packets::s_exch();
		//		memcpy(&exch, data, sizeof FS_packets::s_exch);
		//		//
		//		printf("[ recv s_exch(%i/%i), IPC: %s ]\n", exch.size, sizeof exch, );
		//	}
		//	else printf("[ rcv: %s ]\n\n", data);
		//}

		std::this_thread::sleep_for(1s);
	}

	tcp_client->Disconnect();

	util::c_log::Instance().duo("[ disconnected ]\n");
	system("pause");

	while (true) {}
}