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

	// ���ӿ� �ʿ��� ��� ������Ʈ���� �̸� �ε��Ѵ�.
	HRESULT Ready_Prototype_Component();

	// ���ӿ� �ʿ��� ��� ���� ������Ʈ�� �̸� �ε��Ѵ�.
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

	// ���ӿ� �ʿ��� ��� �ؽ��ĵ��� �̸� �ε��Ѵ�.
	HRESULT Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Texture_SeongUk();  
	HRESULT Ready_Prototype_Texture_MinHee();
	HRESULT Ready_Prototype_Texture_JuHyun();
	HRESULT Ready_Prototype_Texture_HyunWoo();

	// ���ӿ��� ����� ������ �ɼ��� �����Ѵ�.
	HRESULT SetUp_DefaultRenderState();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END



