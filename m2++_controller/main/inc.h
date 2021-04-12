#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <TlHelp32.h>
#include <fstream>
#include <thread>
#include <functional>
#include <WinInet.h>
//
using namespace std::chrono;
//
#include <singleton.h>
//
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "wininet.lib")
//
#include <XorStr.h>
#include <vmp.h>
#include <macro_config.h>
#include <macro.h>
#include <json.h>
#include <log.h>
#include <config.h>
#include <thread.h>