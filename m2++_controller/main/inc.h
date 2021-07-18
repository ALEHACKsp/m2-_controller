#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WINDOWS 1
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
#include <random>
#include <limits>
#include <windowsx.h>
#include <shellapi.h>
//
using namespace std::chrono;
//
#include <singleton.h>
//
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "GuiLite.lib")
//
#include <XorStr.h>
#include <vmp.h>
#include <macro_config.h>
#include <macro.h>
#include <json.h>
#include <log.h>
#include <config.h>
#include <TCPServer.h>
#include <fs_packets.h>
#include <fc_packets.h>
#include <consock.h>
#include <interface.h>
#include <thread.h>