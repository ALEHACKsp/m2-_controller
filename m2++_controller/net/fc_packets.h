#pragma once
#include <inc.h>
namespace net
{
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
		struct s_ping 
		{
			char		message[64 + 1];
		};
	}
}