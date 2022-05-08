#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	typedef struct tagGraphicDesc
	{
		HWND hWnd;
		unsigned int iWinCX;
		unsigned int iWinCY;
	}GRAPHICDESC;

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
	// com 객체 : 언어와의 통신 규약을 갖고 하드웨어를 통제하여 각종 기능을 제공하는 객체

public:
	LPDIRECT3DDEVICE9 Get_Device(void) { 
		return m_pDevice; }

	LPD3DXSPRITE Get_Sprite(void) { 
		return m_pSprite; }

	LPD3DXFONT Get_Font(void) { 
		return m_pFont; }

public:
	HRESULT	InitDevice(HWND hWnd, unsigned int iWinCX, unsigned int iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device);
	void Render_Begin(void);
	void Render_End(void);	

private:
	LPDIRECT3D9				m_p3D = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

	// dx 상에서 2d 이미지를 출력해주는 com객체
	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;

private:
	void SetParameters(HWND hWnd, unsigned int iWinCX, unsigned int iWinCY, D3DPRESENT_PARAMETERS& d3dpp);

public:
	virtual void Free() override;
};

END