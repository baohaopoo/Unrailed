#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
public:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*		m_pGameInstance = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CRenderer*			m_pRenderer = nullptr;

public:
	HRESULT Open_Level(LEVEL eLevelIndex);

	// 게임에 필요한 모든 컴포넌트들을 미리 로딩한다.
	HRESULT Ready_Prototype_Component();

	// 게임에 필요한 모든 게임 오브젝트를 미리 로딩한다.
	HRESULT Ready_Prototype_GameObject();

	HRESULT Ready_Prototype_GameObject_Player();
	HRESULT Ready_Prototype_GameObject_Effect();
	HRESULT Ready_Prototype_GameObject_UI();
	HRESULT Ready_Prototype_GameObject_Camera();
	HRESULT Ready_Prototype_GameObject_InGame();

	HRESULT Ready_Prototype_GameObject_InGame_Loading();
	HRESULT Ready_Prototype_GameObject_InGame_Resource();
	HRESULT Ready_Prototype_GameObject_InGame_Material();
	HRESULT Ready_Prototype_GameObject_InGame_Tool();
	HRESULT Ready_Prototype_GameObject_InGame_Building();
	HRESULT Ready_Prototype_GameObject_InGame_Train();
	HRESULT Ready_Prototype_GameObject_InGame_Netural();
	HRESULT Ready_Prototype_GameObject_InGame_Rail();

	// 게임에 필요한 모든 텍스쳐들을 미리 로딩한다.
	HRESULT Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Texture_SeongUk();  
	HRESULT Ready_Prototype_Texture_MinHee();
	HRESULT Ready_Prototype_Texture_JuHyun();
	HRESULT Ready_Prototype_Texture_HyunWoo();

	// 게임에서 사용할 렌더링 옵션을 세팅한다.
	HRESULT SetUp_DefaultRenderState();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END



