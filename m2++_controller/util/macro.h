#pragma once
//vmp
#ifndef _DEBUG
	#define uvirt VMProtectBeginUltra(__FUNCTION__)
#else
	#define uvirt 
#endif

#ifndef _DEBUG
	#define vmend VMProtectEnd()
#else
	#define vmend
#endif

static const char* rand_string(size_t c)
{
	std::string num_table = XorStr("ABCDEFGHIJKLMNOPQRSTUVXYZabcdefhijklmnopqrstuvwxyz0123456789");
	std::string chr;

	srand((DWORD)GetTickCount64() + (DWORD)&chr + c);
	
	for (auto a =0; a<c; a++)
	{
		const char rc = num_table[rand() % num_table.size()];
		chr.push_back(rc);
	}

	return chr.c_str();
}
static int rand_int(int maxnum = INT_MAX)
{
	auto ret = 0;

	srand((DWORD)GetTickCount64() + (DWORD)&ret + maxnum);
	ret = rand() % maxnum;
	
	return ret;
}

#define randstr(chars)	rand_string(chars)
#define randint(max)	rand_int(max)