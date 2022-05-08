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
생성자
소멸자

public:
Getter();
Setter();

public:
함수

protected:
변수
protected:
함수

private:
변수
private:
함수

public:
생성관련된함수 (Create)
삭제관련된함수 (Free)
};


*/

