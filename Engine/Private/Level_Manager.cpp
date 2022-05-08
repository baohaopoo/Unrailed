#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel * pLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 바꾸기전 현재 있는 레벨이 존재한다면
	if(nullptr != m_pCurrentLevel)
	{
		// 같은 레벨의 오브젝트 매니저가 가지고 있는 오브젝들을 전부 삭제한다.
		if (FAILED(pGameInstance->Clear_LevelResource(m_iLevelIndex)))
			return E_FAIL;
	}

	Safe_Release(m_pCurrentLevel);
	
	// 현재 레벨을 인자로 받은 레벨로 교체한다.
	m_pCurrentLevel = pLevel;

	// 레벨 인덱스 또한 현재 레벨로 교체한다.
	m_iLevelIndex = iLevelIndex;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Manager::ChangeLevel(_uint iLevelIndex)
{
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if ((nullptr != m_ppLevels[iLevelIndex]) && (m_iLevelIndex < m_iMaxLevelIndex)) {
		m_iLevelIndex = iLevelIndex;
		m_pCurrentLevel = m_ppLevels[m_iLevelIndex];

		Safe_Release(pGameInstance);

		return S_OK;
	}

	Safe_Release(pGameInstance);

	return E_FAIL;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

HRESULT CLevel_Manager::Reserve_Manager(_uint iNumLevels)
{
	// 이미 reserve가 된 경우
	if (0 != m_iMaxLevelIndex)
		return E_FAIL;

	// reserve가 되지 않았다면 새로 할당해준다.
	m_ppLevels = new CLevel*[iNumLevels];
	for (int i = 0; i < iNumLevels; ++i)
		m_ppLevels[i] = nullptr;

	// 할당해준 갯수만큼 최대 레벨 인덱스를 설정한다.
	m_iMaxLevelIndex = iNumLevels;

	return S_OK;
}

HRESULT CLevel_Manager::SetLevel(_uint iNumLevels, CLevel* pLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 기존에 존재하던 레벨이 있다면
	if (nullptr != m_ppLevels[iNumLevels])
	{
		// 해당 레벨이 관장하던 레이어 삭제
		if (FAILED(pGameInstance->Clear_LevelResource(iNumLevels)))
			return E_FAIL;
	}

	// 기존 레벨 삭제
	Safe_Release(m_ppLevels[iNumLevels]);

	// 기존 레벨이 위치하던 곳에 인자로 받은 레벨 할당 
	m_ppLevels[iNumLevels] = pLevel;

	// 이 함수에선 현재 포커스 되고 있는 레벨이 바뀌지 않음. 할당만 함.

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Manager::Free()
{
	if (nullptr != m_ppLevels) {
		for (int i = 0; i < m_iMaxLevelIndex; ++i) {
			Safe_Release(m_ppLevels[i]);
		}
	}

	// 다시 release 할 필요가 있나?
	//Safe_Release(m_pCurrentLevel);
}
