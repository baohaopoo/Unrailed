#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEBUTTONSTATE { DIMB_LBUTTON, DIMB_RBUTTON, DIMB_WHEEL, DIMB_END };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;


public:
	bool Get_DIKeyState(_uchar byKeyID) {
		return m_KeyBoardState[byKeyID] < 0;
	}

	_uchar Get_DIMButtonState(MOUSEBUTTONSTATE eDIMBState) {
		return m_MouseState.rgbButtons[eDIMBState];
	}


	_long Get_DIMMoveState(MOUSEMOVESTATE eDIMMState) {
		return ((_long*)&m_MouseState)[eDIMMState];
	}




public:
	HRESULT InitDevice(HINSTANCE hInst, HWND hWnd);
	void SetUp_InputDeviceState();


private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_char					m_KeyBoardState[256];	
	DIMOUSESTATE			m_MouseState;

public:
	virtual void Free() override;
};

END