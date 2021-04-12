#include "log.h"
void util::c_log::setup()
{
	this->fstream.open(this->log_file);
	this->fstream << this->string(XorStr("[ M2++controller launched, binary dated: %s ]\n"), __TIMESTAMP__);
	this->fstream.close();
}
void util::c_log::to_console(const char* in)
{
	printf(in);
}
void util::c_log::to_consolew(const wchar_t* in)
{
	wprintf(in);
}
void util::c_log::to_file(const char* in)
{
	this->fstream.open(this->log_file, std::ios::app);
	if (!this->fstream.is_open()) return;
	this->fstream << in;
	this->fstream.close();
}
void util::c_log::to_filew(const wchar_t* in)
{
	std::wofstream fstream(this->log_file, std::ios::app);
	if (!fstream.is_open()) return;
	fstream << in;
	fstream.close();
}
const char* util::c_log::string(const char* in, ...)
{
	char* buffer = (char*)malloc(sizeof(char) * 4048);
	va_list args;
	va_start(args, in);
	int rc = vsnprintf(buffer, 4048, in, args);
	va_end(args);
	return buffer;
}
void util::c_log::print(const char* in, ...)
{
	char* buffer = (char*)malloc(sizeof(char) * 4048);
	va_list args;
	va_start(args, in);
	int rc = vsnprintf(buffer, 4048, in, args);
	va_end(args);
	this->to_console(buffer);
}
void util::c_log::log(const char* in, ...)
{
	char* buffer = (char*)malloc(sizeof(char) * 4048);
	va_list args;
	va_start(args, in);
	int rc = vsnprintf(buffer, 4048, in, args);
	va_end(args);
	this->to_file(buffer);
}
void util::c_log::duo(const char* in, ...)
{
	char* buffer = (char*)malloc(sizeof(char) * 4048);
	va_list args;
	va_start(args, in);
	int rc = vsnprintf(buffer, 4048, in, args);
	va_end(args);
	this->to_file(buffer);
	this->to_console(buffer);
}

void util::c_log::duow(const wchar_t* in, ...)
{
	wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t) * 4048);
	va_list args;
	va_start(args, in);
	int rc = _vsnwprintf(buffer, 4048, in, args);
	va_end(args);
	this->to_filew(buffer);
	this->to_consolew(buffer);
}
