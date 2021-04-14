#pragma once
#include <inc.h>
namespace net
{

	namespace FS_packets//FromServer
	{
		enum
		{
			OP_INVALID = 0,
			OP_EXCH,
			OP_IDENT,
			OP_PING
		};
		struct s_header
		{
			int			opcode = OP_INVALID;
			int			size = 0;
		};

		struct s_exch : s_header
		{
			s_exch(const char* string)
			{
				opcode = OP_EXCH;
				size = sizeof(s_exch);
				strcpy(ipc_key, string);
			}
			char			ipc_key[64+1] = "<empty>";
		};

		struct s_ping : s_header
		{
			s_ping()
			{
				opcode = OP_PING;
				size = sizeof(s_ping);
			}
			char			ping[4 + 1] = "<>";
		};

		struct s_ident : s_header
		{
			s_ident(ULONGLONG uid)
			{
				opcode = OP_IDENT;
				size = sizeof(s_ident);
				ident = uid;
			}
			ULONGLONG		ident = 0;
		};
	}
}