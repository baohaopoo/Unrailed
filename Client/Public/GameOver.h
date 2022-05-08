#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)
class GameOver final : public CLevel
{
public:
	explicit GameOver(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~GameOver() = default;


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
public:
	static GameOver* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END

