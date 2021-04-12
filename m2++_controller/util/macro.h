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