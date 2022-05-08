#pragma once

#include "Component.h"

/* 1. 객체들을 그리는순서에 따라 그룹지어 모아놓는다. */
/* 2. 모아놓은 객체들의 렌더함수를 호출해준다.  */
/* 3. 그룹에 맞는 렌더상태를 설정한다.  */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { GROUP_PRIORITY, GROUP_NONBLEND, GROUP_ALPHABLEND, GROUP_UI, GROUP_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup_UI(class CGameObject* pGameObject);
	HRESULT Add_RenderGroup_NonCulling(RENDERGROUP eGroup, class CGameObject* pGameObject);
	HRESULT Render_GameObjects();


private:
	list<class CGameObject*>			m_RenderList[GROUP_END];
	typedef list<class CGameObject*>	RENDERLIST;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();


public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

