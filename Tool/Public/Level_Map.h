#pragma once

#include "Tool_Defines.h"
#include "Level.h"
#include "GameObject.h"

BEGIN(Tool)

class CLevel_Map final : public CLevel
{
public:
	enum MODE { MODE_CREATE, MODE_SELECT, MODE_CAMERA, MODE_END };

private:
	explicit CLevel_Map(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Map() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);

public:
	static CLevel_Map* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	HRESULT CreateMapObject(void);
	HRESULT DeleteMapObject(void);

public:
	// 모드 관련 함수
	void changeMode(MODE _eModeType) { m_eCurrMode = _eModeType; }
	MODE GetCurrMode(void) { return m_eCurrMode; }
	CGameObject* GetSelectObject(void) { return m_pSelectObject; }

private:
	bool	m_bLButtonClick = false;
	bool	m_bRButtonClick = false;

	MODE	m_eCurrMode;
	CGameObject* m_pSelectObject = nullptr;
};

END