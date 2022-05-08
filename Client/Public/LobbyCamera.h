#pragma once
#include "Client_Defines.h"
#include "Camera.h"
class LobbyCamera final : public CCamera
{
private:
	explicit LobbyCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit LobbyCamera(const LobbyCamera& rhs);
	virtual ~LobbyCamera() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	static LobbyCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

