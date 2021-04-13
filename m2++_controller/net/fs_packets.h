#pragma once
#include <inc.h>
namespace net
{

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

		struct s_exch : s_header
		{
			char			ipc_key[64+1] = "<empty>";
		};
	}
}