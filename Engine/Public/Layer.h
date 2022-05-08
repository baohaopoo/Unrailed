#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Find_Object(const string& _strObjectName);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	list<class CGameObject*>& Get_ObjectList(void) { return m_ObjectList; }

private:
	list<class CGameObject*>			m_ObjectList;
	typedef list<class CGameObject*>	OBJECTLIST;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END