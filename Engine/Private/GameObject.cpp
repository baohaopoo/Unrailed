#include "..\Public\GameObject.h"

#include "GameInstance.h"



CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device, const string _ObjectName)
	: m_pGraphic_Device(pGraphic_Device), m_ObjectName(_ObjectName)
	, m_iTextureNumber(0), m_bDead(false), m_eType(OBJECT_TYPE::OBJECT_END)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: 
	m_pGraphic_Device(rhs.m_pGraphic_Device), m_ObjectName(rhs.m_ObjectName),
	m_iTextureNumber(rhs.m_iTextureNumber), m_bDead(rhs.m_bDead), m_eType(rhs.m_eType)
{
	Safe_AddRef(m_pGraphic_Device);
}

void CGameObject::Delete_Component(const _tchar * pComponentTag)
{
	// 기존에 컴포넌트가 존재할 경우
	if (m_Components[pComponentTag] != nullptr)
		m_Components.erase(pComponentTag);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	CComponent*	pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pGraphic_Device);
}
