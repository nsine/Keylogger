// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // !_WINSOCKAPI_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <string>
#include <cstdlib>
#include <memory>
#include <io.h>
#include <iostream>
#include <fcntl.h>
#include <thread>
#include <tchar.h>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>
#include <codecvt>
#include <locale>
#include <functional>
#include <map>
#include <set>

#include <algorithm>
#include <iterator>
#include <regex>
#include <winsock2.h>
#include <WS2tcpip.h>

#include <winuser.h>
#include <windows.h>
#include <windowsx.h>
	
#include <Psapi.h>

#pragma warning(push, 0)        
#include "CSmtp.h"
#pragma warning(pop)

// TODO: reference additional headers your program requires here
