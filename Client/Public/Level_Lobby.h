#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	explicit CLevel_Lobby(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Btn(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ProgressBar(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ProgressBarExit(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BtnExit(const _tchar* pLayerTag);
	HRESULT Ready_Layer_TalkBallon(const _tchar* pLayerTag);
	
private:
	LEVEL			m_eNextLevel = LEVEL_END;
	class CLoader*	m_pLoader = nullptr;
	LPD3DXFONT font;
	LPD3DXFONT font2;
public:
	static CLevel_Lobby* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};

END