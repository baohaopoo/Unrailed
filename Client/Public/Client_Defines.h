#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1280;
	const unsigned int	g_iWinCY = 720;

	enum LEVEL { 
		LEVEL_STATIC, LEVEL_LOADING, 

		LEVEL_LOGO, LEVEL_LOBBY, LEVEL_GAMEPLAY, LEVEL_GAMEPLAY2,LEVELGAMEPLAY3,LEVEL_GAMEOVER,
		
		LEVEL_END };
}

#include <process.h>
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"


extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

/*

class CMainApp
{
private:
������
�Ҹ���

public:
Getter();
Setter();

public:
�Լ�

protected:
����
protected:
�Լ�

private:
����
private:
�Լ�

public:
�������õ��Լ� (Create)
�������õ��Լ� (Free)
};


*/

