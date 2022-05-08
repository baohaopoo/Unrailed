#include "..\Public\Layer.h"
#include "GameObject.h"


CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_ObjectList.size())
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}

CGameObject * CLayer::Find_Object(const string & _strObjectName)
{
	auto	iter = find_if(m_ObjectList.begin(), m_ObjectList.end(), [_strObjectName](const CGameObject* pGameObject) {
		return pGameObject->Get_ObjectName() == _strObjectName;
	});

	if (iter == m_ObjectList.end())
		return nullptr;

	return *iter;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_ObjectList)
		pGameObject->Tick(fTimeDelta);

}

void CLayer::LateTick(_float fTimeDelta)
{
	auto& iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end();) {

		(*iter)->LateTick(fTimeDelta);
	
		if ((*iter)->Get_Dead()) {
			m_ObjectList.erase(iter++);
		}
		else
			++iter;
	}
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
		Safe_Release(pGameObject);

	m_ObjectList.clear();

}
