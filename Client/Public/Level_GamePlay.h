#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	explicit CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Giant(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MenuBtnUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MSignUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_RightHand(const _tchar * pLayerTag);
	HRESULT Ready_Layer_LeftHand(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Hammer(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_PickEx(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Bucket(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Rock(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Tree(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Bird(const _tchar * pLayerTag);
	HRESULT	Ready_Layer_Material_Wood(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Material_Iron(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Material_Rail_Straight(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Dynamite(const _tchar * pLayerTag);
	HRESULT Ready_Layer_MissionBtn(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Rail_Invisible(const _tchar * pLayerTag);

private:
	HRESULT Load_MapToolData(const _tchar* _pMapFileName);

private:
	bool m_isCol = false;
	LPD3DXFONT font;


public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END