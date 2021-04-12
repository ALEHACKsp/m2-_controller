#pragma once
#include <inc.h>
namespace boot
{
	namespace conf_json
	{
		class c_config
		{
		public:
			std::wstring	data_dir;	//path for map files, server configs, address dumps - passed to ingame bin post-inject
			std::string		ipc_port;	//port used for ipc client <--> controller
			std::string		ipc_comkey;	//key used to xor packets, random each launch - passed to igname bin post-inject
			std::string		username;	//auth user - xor by ipc_comkey
			std::string		password;	//auth pass - xor by ipc_comkey
			std::string		window_x;	//last exit pos
			std::string		window_y;	//last exit pos
			c_config() {}
			c_config(const std::wstring a1, const std::string a2, const std::string a3, const std::string a4, const std::string a5, const std::string a6, const std::string a7) :
				data_dir(a1), ipc_port(a2), ipc_comkey(a3), username(a4), password(a5), window_x(a6), window_y(a7)
			{ }
			NLOHMANN_DEFINE_TYPE_INTRUSIVE(c_config, data_dir, ipc_port, ipc_comkey, username, password, window_x, window_y);
		};
	}
	class c_conf : public s<c_conf>
	{
	public:
		conf_json::c_config	base_config;
	public:
		bool	save();
		bool	load();
		void	setup();
	};
}