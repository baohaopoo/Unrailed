#pragma once

#include "Component.h"

/* 1. 객체들을 그리는순서에 따라 그룹지어 모아놓는다. */
/* 2. 모아놓은 객체들의 렌더함수를 호출해준다.  */
/* 3. 그룹에 맞는 렌더상태를 설정한다.  */

BEGIN(Engine)

class ENGINE_DLL CCollision final : public CComponent
{
private:
	explicit CCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollision() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_CollisionList(_uint _iNumLevel, class CGameObject* pGameObject);
	HRESULT Collision_GameObjects();


private:
	list<class CGameObject*>*			m_CollisionList;
	typedef list<class CGameObject*>	RENDERLIST;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();


public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

