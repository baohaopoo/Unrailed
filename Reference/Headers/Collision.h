#pragma once

#include "Component.h"

/* 1. ��ü���� �׸��¼����� ���� �׷����� ��Ƴ��´�. */
/* 2. ��Ƴ��� ��ü���� �����Լ��� ȣ�����ش�.  */
/* 3. �׷쿡 �´� �������¸� �����Ѵ�.  */

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

