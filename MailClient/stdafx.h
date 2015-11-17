// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// Global definitions
#define IDS_FIRSTCOLUMN 0

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <Commctrl.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>


// TODO: reference additional headers your program requires here
#include "curl\curl.h"


// Link curl Lib file
#pragma comment(lib, "curl\\lib\\libcurl.lib")
#pragma comment(lib, "Comctl32.lib")