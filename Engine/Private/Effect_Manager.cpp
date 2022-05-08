#include "..\Public\Effect_Manager.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

CComponent * CEffect_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;
		

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);	
}

CGameObject * CEffect_Manager::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	return nullptr;
}

HRESULT CEffect_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (0 != m_iNumLevels)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CEffect_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype || 
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));	

	return S_OK;
}

HRESULT CEffect_Manager::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CEffect_Manager::Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * _pObject)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();
	
	auto& iter = find_if(ObjectList.begin(), ObjectList.end(), [_pObject](CGameObject* pObject) -> bool {
		return _pObject == pObject;
	});

	if (iter == ObjectList.end())
		return E_FAIL;

	ObjectList.erase(iter);

	return S_OK;
}

HRESULT CEffect_Manager::Delete_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return S_OK;

	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	for (auto& Object : ObjectList)
		Safe_Release(Object);

	Safe_Release(pLayer);

	m_pLayers[iLevelIndex].erase(pLayerTag);

	return S_OK;
}

void CEffect_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);
		}
	}
}

void CEffect_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateTick(fTimeDelta);
		}
	}
}

HRESULT CEffect_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

HRESULT CEffect_Manager::Save_Data(const TCHAR * _strFilePath, CGameObject* _pGameObject)
{
	// 인자로 받은 경로에 특정 오브젝트의 정보를 저장하는 함수
	map<const _tchar*, class CComponent*> Components = _pGameObject->Get_Components();
	for (auto& component : Components) {
		//component.second
	}

	// 저장 방식은 오브젝트의 이름을 먼저 저장하고, 오브젝트가 가지고 있는 컴포넌트의 정보를 하나씩 돌아가며 저장하는 식으로 한다.

	return S_OK;
}

HRESULT CEffect_Manager::Load_Data(const TCHAR* _strFilePath, _uint iLevelIndex, const _tchar* pLayerTag)
{
	// 인자로 받은 경로에 있는 파일을 로드하여 오브젝트를 만들고,
	// 해당 오브젝트를 인자로 받은 레벨에 있는 인자로 받은 레이어에 집어넣는 함수 

	int len = wcslen(_strFilePath);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, _strFilePath, 2 * len + 1);
	std::string s = psz;

	delete[] psz;

	// 파일을 연다
	ifstream in(s);

	// 파일이 존재한다면
	if (in) {
		while (!in.eof()) {
			// 한줄을 읽는다. 여기에 저장된 오브젝트의 VIBuffer 정보가 담겨있다.
			int eType;
			in >> eType;

			VTXTEX* pVertices = nullptr;
			VTXCUBETEX* pCubeVertices = nullptr;

			switch ((VIBuffer)eType)
			{
			case Engine::VIB_RECT:
				pVertices = new VTXTEX[4];

				break;
			case Engine::VIB_CUBE:
				pCubeVertices = new VTXCUBETEX[8];
				for (int i = 0; i < 8; ++i) {
					in >> pCubeVertices[i].vPosition.x;
					in >> pCubeVertices[i].vPosition.y;
					in >> pCubeVertices[i].vPosition.z;
				}
				break;
			case Engine::VIB_TERRAIN:
				break;
			}

		}
	}

	return S_OK;
}

void CEffect_Manager::Tick(_uint iNumLevel, _float fTimeDelta)
{
	for (auto& Pair : m_pLayers[iNumLevel])
	{
		Pair.second->Tick(fTimeDelta);
	}
}

void CEffect_Manager::LateTick(_uint iNumLevel, _float fTimeDelta)
{
	for (auto& Pair : m_pLayers[iNumLevel])
	{
		Pair.second->LateTick(fTimeDelta);
	}
}

map<const _tchar*, class CLayer*>& CEffect_Manager::Find_LevelLayers(_uint iLevelIndex)
{
	return m_pLayers[iLevelIndex];
}

CGameObject * CEffect_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));
	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CEffect_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CGameObject * CEffect_Manager::Find_Object(_uint iLevelIndex, const _tchar * pLayerTag, const string & _strObjectName)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second->Find_Object(_strObjectName);
}

void CEffect_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
