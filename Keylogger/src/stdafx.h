// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable : 4996)  

#include "targetver.h"

#include <cstdlib>
#include <memory>
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <map>
#include <set>

#include <functional>
#include <algorithm>
#include <iterator>
#include <regex>

#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <codecvt>
#include <locale>

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // !_WINSOCKAPI_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <winuser.h>
#include <Psapi.h>

#include "../vendor/csmtp/CSmtp.h"

// TODO: reference additional headers your program requires here
