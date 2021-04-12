#include "config.h"
void boot::c_conf::setup()
{
	uvirt;
	const auto did_load = this->load();
	util::c_log::Instance().duo(XorStr("[ base config load result > %s ]\n"), did_load ? "ok" : "failed");
	if (!did_load)
	{
		//set data dir, port, ipc key
		
	}
	else
	{
		//re-randomize port, ipc key
	}
	vmend;
}

bool boot::c_conf::save()
{
	std::ofstream	bconf(XorStr("M2++CONTROLLER_BASE.CFG"));
	if (!bconf.is_open()) return FALSE;
	
	auto j = nlohmann::json();
	nlohmann::to_json(j, this->base_config);

	bconf << j << "\n";

	bconf.close();

	return TRUE;
}

bool boot::c_conf::load()
{
	std::ifstream	bconf(XorStr("M2++CONTROLLER_BASE.CFG"));
	if (!bconf.is_open()) return FALSE;
	
	std::string		filebuf;
	std::getline(bconf, filebuf);
	if (filebuf.empty()) return FALSE;
	
	auto jparse = nlohmann::json::parse(filebuf);
	this->base_config = jparse.get<boot::conf_json::c_conf>();

	bconf.close();

	return TRUE;
}
