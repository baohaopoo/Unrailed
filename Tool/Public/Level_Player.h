#pragma once

#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class CLevel_Player final : public CLevel
{
private:
	explicit CLevel_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Player() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);

public:
	static CLevel_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END