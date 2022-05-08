#pragma once

namespace Tool {
	const unsigned int g_iWinCX = 1280;
	const unsigned int g_iWinCY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_MAP, LEVEL_OBJECT, LEVEL_PLAYER, LEVEL_EFFECT, LEVEL_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern POINT Resolution;
extern float g_ToolViewAspect;

#include <math.h>

using namespace Tool;

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }

